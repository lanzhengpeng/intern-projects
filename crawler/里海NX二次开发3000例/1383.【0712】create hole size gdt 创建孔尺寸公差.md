### 【0712】create hole size gdt 创建孔尺寸公差

#### 代码

```cpp
    /*HEAD CREATE_HOLE_SIZE_GDT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_vec.h>  
    #include <uf_modl.h>  
    #include <uf_trns.h>  
    #include <uf_view.h>  
    #include <uf_csys.h>  
    #include <uf_gdt.h>  
    #include <uf_part.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 译:根据原文内容，UF_print_syslog是V18版本新增的函数。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
    static int mask_for_cylindrical_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_CYLINDRICAL_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static void map_abs2view(double c[3])  
    {  
        char  
            vname[30];  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(abs, vw, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void map_view2abs(double c[3])  
    {  
        char  
            vname[30];  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(vw, abs, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void map_abs2wcs(double *cc)  
    {  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            wcs[12],  
            mx[12];  
        int  
            irc;  
        tag_t  
            wcs_ent,  
            wcs_mx;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs_ent));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs_ent, &wcs_mx, wcs));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, &wcs[3]));  
        FTN(uf5940)(abs, wcs, mx, &irc);  
        FTN(uf5941)(cc, mx);  
    }  
    static tag_t select_point_on_cylindrical_face(char *prompt, double cp[3])  
    {  
        int  
            n,  
            resp;  
        tag_t  
            face,  
            line,  
            object,  
            view;  
        double  
            tol;  
        UF_CURVE_line_t  
            lp;  
        UF_MODL_intersect_info_p_t  
            *ints;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_cylindrical_faces, NULL,  
                &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            map_abs2view(cp);  
            lp.start_point[0] = cp[0];  
            lp.start_point[1] = cp[1];  
            lp.start_point[2] = cp[2] + 1000;  
            lp.end_point[0] = cp[0];  
            lp.end_point[1] = cp[1];  
            lp.end_point[2] = cp[2] - 1000;  
            map_view2abs(lp.start_point);  
            map_view2abs(lp.end_point);  
            UF_CALL(UF_CURVE_create_line(&lp, &line));  
            face = object;  
            UF_MODL_ask_distance_tolerance(&tol);  
            UF_CALL(UF_MODL_intersect_objects(face, line, tol, &n, &ints));  
            if (n > 0)  
            {  
                UF_VEC3_copy(ints[0]->intersect.point.coords, cp);  
                UF_free(ints);  
            }  
            UF_CALL(UF_OBJ_delete_object(line));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static double ask_cylindrical_face_diameter(tag_t face, double pos[3])  
    {  
        double  
            parm[2],  
            pnt[3],  
            dirU1[3],  
            dirU2[3],  
            dirV1[3],  
            dirV2[3],  
            norm_dir[3],  
            radii[2];  
        UF_CALL(UF_MODL_ask_face_parm(face, pos, parm, pnt));  
        UF_CALL(UF_MODL_ask_face_props(face, parm, pnt, dirU1, dirV1, dirU2,  
            dirV2, norm_dir, radii));  
        return 2*radii[0];  
    }  
    static int mask_for_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_CIRCULAR_EDGE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static void ask_pos_on_obj(tag_t obj, double loc[3])  
    {  
        tag_t  
            line;  
        double  
            cp[3],  
            dist;  
        UF_CURVE_line_t  
            lp;  
        map_abs2view(loc);  
        lp.start_point[0] = loc[0];  
        lp.start_point[1] = loc[1];  
        lp.start_point[2] = loc[2] + 1000;  
        lp.end_point[0] = loc[0];  
        lp.end_point[1] = loc[1];  
        lp.end_point[2] = loc[2] - 1000;  
        map_view2abs(lp.start_point);  
        map_view2abs(lp.end_point);  
        UF_CALL(UF_CURVE_create_line(&lp, &line));  
        UF_CALL(UF_MODL_ask_minimum_dist(obj, line, 0, cp, 0, cp, &dist, loc, cp));  
        UF_CALL(UF_OBJ_delete_object(line));  
    }  
    static tag_t select_point_on_edge(char *prompt, double cp[3])  
    {  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_edges, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            ask_pos_on_obj(object, cp);  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void set_wcs_to_view(char *view_name)  
    {  
        tag_t  
            matobj,                     /* Matrix object 译:根据上下文，"Matrix object" 可翻译为 "矩阵对象"。 */  
            csys;                       /* Csys object  译:系统对象 */  
        double  
            origin[3],                  /* View's center 译:View的center翻译为视图的中心。 */  
            matrix[9],                  /* View's matrix 译:视图的矩阵 */  
            scale;  
        UF_CALL(uc6430(view_name, origin, &scale));  
        UF_CALL(uc6433(view_name, matrix));  
        if (!UF_CALL( UF_CSYS_create_matrix(matrix, &matobj) ) &&  
            !UF_CALL( UF_CSYS_create_temp_csys(origin, matobj, &csys) ))  
            UF_CALL( UF_CSYS_set_wcs(csys) );  
    }  
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    #define WRITE_P(X) (write_pointer_to_listing_window(#X, X))  
    static void write_pointer_to_listing_window(char *title, void *pointer)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (pointer == NULL)  
            sprintf(msg, "%s = NULL\n", title);  
        else  
            sprintf(msg, "%s != NULL\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITEnD(n,X) (write_integer_array_to_listing_window(#X, X, 1, n))  
    static void write_integer_array_to_listing_window(char *name, int *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%d", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void report_gdt_modifier_data(UF_GDT_modifier_data_p_t modifier_data)  
    {  
        int  
            ii;  
        if (modifier_data == NULL) return;  
        WRITE_D(modifier_data->num_modifiers);  
        WRITEnD(modifier_data->num_modifiers,  
            (int*)modifier_data->modifier_types);  
        WRITE_P(modifier_data->proj_zone);  
        if (modifier_data->proj_zone != NULL)  
        {  
            WRITE_D(modifier_data->proj_zone->expression);  
            WRITE_F(modifier_data->proj_zone->value);  
            WRITE_D(modifier_data->proj_zone->decimal_format);  
            WRITE_D(modifier_data->proj_zone->direction);  
        }  
        WRITE_P(modifier_data->unit_length);  
        if (modifier_data->unit_length != NULL)  
        {  
            WRITE_D(modifier_data->unit_length->tolerance_expression);  
            WRITE_D(modifier_data->unit_length->length_expression);  
            WRITE_D(modifier_data->unit_length->width_expression);  
            WRITE_D(modifier_data->unit_length->decimal_format);  
            WRITE_D(modifier_data->unit_length->ub_decimal_format);  
        }  
        WRITE_P(modifier_data->unit_area);  
        if (modifier_data->unit_area != NULL)  
        {  
            WRITE_D(modifier_data->unit_area->tolerance_expression);  
            WRITE_D(modifier_data->unit_area->length_expression);  
            WRITE_D(modifier_data->unit_area->width_expression);  
            WRITE_D(modifier_data->unit_area->decimal_format);  
            WRITE_D(modifier_data->unit_area->ub_decimal_format);  
        }  
        WRITE_P(modifier_data->bonus_tol);  
        if (modifier_data->bonus_tol != NULL)  
        {  
            WRITE_D(modifier_data->bonus_tol->expression);  
            WRITE_F(modifier_data->bonus_tol->value);  
            WRITE_D(modifier_data->bonus_tol->decimal_format);  
        }  
        WRITE_D(modifier_data->num_keywords);  
        for (ii = 0; ii < modifier_data->num_keywords; ii++)  
        {  
            WRITE_D(modifier_data->keywords[ii].id);  
        }  
    }  
    static void report_gdt_feature_parms(tag_t tolerance_feature)  
    {  
        int  
            ii;  
        UF_GDT_feature_parms_p_t  
            feat_parms;  
        if (UF_CALL(UF_GDT_ask_feature_parms(tolerance_feature, &feat_parms)))  
            return;  
        WRITE_D(feat_parms->num_faces);  
        WRITEnD(feat_parms->num_faces, (int *)feat_parms->face_list);  
        WRITE_S(feat_parms->descript->name);  
        WRITE_S(feat_parms->descript->description);  
        WRITE_D(feat_parms->feature);  
        WRITE_D(feat_parms->pattern);  
        WRITE_P(feat_parms->modifier_data);  
        report_gdt_modifier_data(feat_parms->modifier_data);  
        WRITE_D(feat_parms->num_sets);  
        for (ii = 0; ii < feat_parms->num_sets; ii++)  
        {  
            WRITE_D(feat_parms->sets[ii]->num_faces);  
            WRITEnD(feat_parms->sets[ii]->num_faces,  
                (int *)feat_parms->sets[ii]->faces);  
        }  
        WRITE_D(feat_parms->def.type);  
        WRITE_D(feat_parms->def.so_tag);  
        WRITE_F(feat_parms->def.radius);  
        WRITE_D(feat_parms->def.trimmed_sheet);  
        UF_CALL(UF_GDT_free(UF_GDT_FEAT_PARMS_TYPE, feat_parms));  
    }  
    static void report_gdt_size_tolerance_parms(tag_t size_tol_feat)  
    {  
        UF_GDT_size_tolerance_p_t  
            size_data;  
        if (UF_CALL(UF_GDT_ask_size_tolerance_parms(size_tol_feat, &size_data)))  
            return;  
        WRITE_D(size_data->value_type);  
        WRITE_D(size_data->dimension);  
        WRITE_F(size_data->nominal_value);  
        WRITE_D(size_data->upper_tol.expression);  
        WRITE_F(size_data->upper_tol.value);  
        WRITE_D(size_data->upper_tol.decimal_format);  
        WRITE_D(size_data->lower_tol.expression);  
        WRITE_F(size_data->lower_tol.value);  
        WRITE_D(size_data->lower_tol.decimal_format);  
        WRITE_D(size_data->tol_format);  
        WRITE_P(size_data->modifier_data);  
        report_gdt_modifier_data(size_data->modifier_data);  
        WRITE_D(size_data->decimal_format);  
        UF_CALL(UF_GDT_free(UF_GDT_SIZE_TOL_TYPE, size_data));  
    }  
    static void do_it(void)  
    {  
        int  
            ii = 0;  
        tag_t  
            edge,  
            faces[1],  
            orig_wcs,  
            size_tol,  
            tol_inst,  
            view;  
        double  
            e_pos[3],  
            f_pos[3],  
            orig[3];  
        char  
            feat_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_GDT_feature_parms_t  
            feat_parms;  
        UF_GDT_geometric_definition_t  
            geo_def = {UF_GDT_ALL_GEOMETRY, NULL_TAG, 0.0, NULL_TAG };  
        UF_GDT_size_tolerance_t  
            size_data;  
        UF_GDT_description_t  
            descript;  
        UF_GDT_modifier_data_t  
            modifier_data;  
        UF_GDT_surface_parms_t  
            set;  
        UF_GDT_surface_parms_p_t  
            sets[1];  
        UF_CALL(UF_CSYS_ask_wcs(&orig_wcs));  
        set_wcs_to_view("");  
        UF_CALL(UF_VIEW_ask_work_view(&view));  
        UF_CALL(UF_GDT_init(UF_GDT_FEAT_PARMS_TYPE, &feat_parms));  
        UF_CALL(UF_GDT_init(UF_GDT_SIZE_TOL_TYPE, &size_data));  
        UF_CALL(UF_GDT_init(UF_GDT_MOD_DATA_TYPE, &modifier_data));  
        UF_CALL(UF_GDT_init(UF_GDT_SURFACE_PARMS_TYPE, &set));  
        feat_parms.num_faces = 1;  
        feat_parms.face_list = faces;  
        feat_parms.descript = &descript;  
        feat_parms.descript->name = feat_name;  
        feat_parms.descript->description = "Hole Size Feature";  
        feat_parms.feature = UF_GDT_HOLE_FEATURE;  
        feat_parms.pattern = UF_GDT_NO_PATTERN;  
        feat_parms.modifier_data = &modifier_data;  
        feat_parms.num_sets = 1;  
        sets[0] = &set;  
        feat_parms.sets = sets;  
        feat_parms.sets[0]->num_faces = 1;  
        feat_parms.sets[0]->faces = faces;  
        feat_parms.def = geo_def;  
        size_data.value_type = UF_GDT_DIAMETRAL_SIZE;  
        size_data.dimension = NULL_TAG;  
        size_data.nominal_value = 0.0;  /* filled in later 译:填空题 */  
        UF_CALL(UF_MODL_create_exp_tag("0.25", &size_data.upper_tol.expression));  
        size_data.upper_tol.value = 0.25;  
        size_data.upper_tol.decimal_format = 2;  
        UF_CALL(UF_MODL_create_exp_tag("-.25", &size_data.lower_tol.expression));  
        size_data.lower_tol.value = -.25;  
        size_data.lower_tol.decimal_format = 2;  
        size_data.tol_format = UF_GDT_PLUS_OVER_MINUS;  
        size_data.modifier_data = &modifier_data;  
        size_data.decimal_format = 2;  
        while (  
            ((faces[0] = select_point_on_cylindrical_face("Select hole face",  
                f_pos)) != NULL_TAG) &&  
            ((edge = select_point_on_edge("Select edge for tolerance", e_pos))  
                != NULL_TAG))  
        {  
            size_data.nominal_value = ask_cylindrical_face_diameter(faces[0],f_pos);  
            do  
            {  
                size_tol = NULL_TAG;  
                sprintf(feat_name, "HoleFeature%d", ++ii);  
                UF_CALL(UF_OBJ_cycle_by_name(feat_name, &size_tol));  
            } while (size_tol != NULL_TAG);  
            if (UF_CALL(UF_GDT_create_size_tolerance(&feat_parms, &size_data,  
                &size_tol))) continue;  
            WRITE_D(size_tol);  
            report_gdt_feature_parms(size_tol);  
            report_gdt_size_tolerance_parms(size_tol);  
            UF_VEC3_copy(e_pos, orig);  
            map_abs2view(orig);  
            orig[0] = orig[0] + 0.5;  
            orig[1] = orig[1] + 0.5;  
            map_view2abs(orig);  
            display_temporary_point(orig);  
            map_abs2wcs(orig);  
            display_temporary_point(e_pos);  
            map_abs2view(e_pos);  
        /*  This may fail - see PR 4608170 译:这可能会失败 - 见 PR 4608170。 */  
            UF_CALL(UF_GDT_create_instance(UF_GDT_SIZE_TOLERANCE_TYPE, size_tol,  
                view, edge, orig, e_pos, &tol_inst));  
        }  
        UF_CALL(UF_CSYS_set_wcs(orig_wcs));  
    }  
    /* 里海 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++程序，其主要功能是创建一个孔尺寸公差特征。
>
> 主要步骤包括：
>
> 1. 初始化NX Open库
> 2. 设置工作坐标系到当前视图
> 3. 初始化孔特征参数，包括名称、描述、特征类型等
> 4. 初始化孔尺寸公差参数，包括值类型、公差格式、上下公差等
> 5. 循环让用户选择圆柱面和边缘
> 6. 获取圆柱面直径作为公差的名义值
> 7. 创建孔尺寸公差特征
> 8. 输出公差特征和参数信息
> 9. 创建公差特征实例
> 10. 重置工作坐标系
> 11. 终止NX Open库
>
> 代码使用了NX Open的UF、UI、MODL、GDT等模块，实现了孔尺寸公差的创建和显示。整体结构清晰，步骤明确，是一个典型的NX Open二次开发示例。
>
