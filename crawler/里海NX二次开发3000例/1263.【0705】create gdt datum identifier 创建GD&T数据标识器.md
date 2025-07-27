### 【0705】create gdt datum identifier 创建GD&T数据标识器

#### 代码

```cpp
    /*HEAD CREATE_GDT_DATUM_IDENTIFIER CCC UFUN */  
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
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：
注意：UF_print_syslog 是 V18 新增的，请只提供翻译，不要添加其他内容。 */  
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
    /* 里海 */  
    static int mask_for_planar_face(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_PLANAR_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_planar_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select planar face", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART,  
            mask_for_planar_face, NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        return NULL_TAG;  
    }  
    static logical is_item_on_list(uf_list_p_t object_list, tag_t item)  
    {  
        int  
            ii;  
        uf_list_p_t  
            temp;  
        for (ii = 0, temp = object_list; temp != NULL; temp = temp->next, ii++)  
            if (temp->eid == item) return TRUE;  
        return FALSE;  
    }  
    /* 里海 */  
    static int filter_face_edges(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        tag_t  
            the_face = *(tag_t *)data;  
        uf_list_p_t  
            face_list;  
        UF_CALL(UF_MODL_ask_edge_faces(object, &face_list));  
        if (is_item_on_list(face_list, the_face))  
        {  
            UF_CALL(UF_MODL_delete_list(&face_list));  
            return (UF_UI_SEL_ACCEPT);  
        }  
        else  
        {  
            UF_CALL(UF_MODL_delete_list(&face_list));  
            return (UF_UI_SEL_REJECT);  
        }  
    }  
    /* 里海 */  
    static int mask_for_face_edges(UF_UI_selection_p_t select, void *data)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_face_edges, NULL, data)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static void map_abs2view(double c[3])  
    {  
        char  
            vname[30];  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(abs_mx, vw, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void map_view2abs(double c[3])  
    {  
        char  
            vname[30];  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(vw, abs_mx, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void map_abs2wcs(double *cc)  
    {  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
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
        FTN(uf5940)(abs_mx, wcs, mx, &irc);  
        FTN(uf5941)(cc, mx);  
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
    static tag_t select_point_on_edge_of_face(char *prompt, tag_t face,  
        double cp[3])  
    {  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select edge of face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_face_edges, &face, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            ask_pos_on_obj(object, cp);  
            return object;  
        }  
        return NULL_TAG;  
    }  
    static void set_wcs_to_view(char *view_name)  
    {  
        tag_t  
            matobj,                     /* Matrix object 译:矩阵对象 */  
            csys;                       /* Csys object  译:The Csys object is used to describe a coordinate system in a CAD system, including its origin, orientation, and units. It is typically used to define the world coordinate system or the coordinate system of a particular drawing or part. The Csys object may also be used to define the coordinate systems of toolpaths and machine tools in CAM systems. In summary, it is an object used to define and manage coordinate systems in CAD/CAM applications. */  
        double  
            origin[3],                  /* View's center 译:View的中心 */  
            matrix[9],                  /* View's matrix 译:翻译如下：

视图的矩阵 */  
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
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void report_gdt_modifier_data(UF_GDT_modifier_data_p_t modifier_data)  
    {  
        logical  
            is_user_keyword;  
        int  
            ii;  
        tag_t  
            part = UF_ASSEM_ask_work_part();  
        char  
            *keyword_name,  
            *keyword_text;  
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
            UF_CALL(UF_GDT_is_user_defined_keyword(part,  
                modifier_data->keywords[ii].id, &is_user_keyword));  
            WRITE_L(is_user_keyword);  
            UF_CALL(UF_GDT_ask_keyword_name(part, modifier_data->keywords[ii].id,  
                &keyword_name));  
            WRITE_S(keyword_name);  
            UF_CALL(UF_GDT_ask_keyword_text(part, keyword_name, &keyword_text));  
            WRITE_S(keyword_text);  
            UF_free(keyword_text);  
            UF_free(keyword_name);  
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
    static void report_gdt_datum_identifier_parms(tag_t datum_feat)  
    {  
        UF_GDT_datum_identifier_p_t  
            datum_data;  
        if (UF_CALL(UF_GDT_ask_datum_ident_parms(datum_feat, &datum_data)))  
            return;  
        WRITE_S(datum_data->datum_label);  
        WRITE_L(datum_data->is_individual);  
        UF_CALL(UF_GDT_free(UF_GDT_DATUM_IDENT_TYPE, datum_data));  
    }  
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            ii = 0;  
        tag_t  
            edge,  
            faces[1],  
            orig_wcs,  
            datum_feat,  
            tol_inst,  
            view;  
        double  
            e_pos[3],  
            orig[3];  
        char  
            feat_name[MAX_ENTITY_NAME_SIZE+1],  
            label[10] = { "AAA" };  
        UF_GDT_feature_parms_t  
            feat_parms;  
        UF_GDT_geometric_definition_t  
            geo_def = {UF_GDT_ALL_GEOMETRY, NULL_TAG, 0.0, NULL_TAG };  
        UF_GDT_description_t  
            descript;  
        UF_GDT_modifier_data_t  
            modifier_data;  
        UF_GDT_datum_identifier_t  
            datum_data;  
        UF_GDT_keyword_t  
            keywords[1];  
        UF_GDT_surface_parms_t  
            set;  
        UF_GDT_surface_parms_p_t  
            sets[1];  
        UF_CALL(UF_CSYS_ask_wcs(&orig_wcs));  
        set_wcs_to_view("");  
        UF_CALL(UF_VIEW_ask_work_view(&view));  
        UF_CALL(UF_GDT_init(UF_GDT_FEAT_PARMS_TYPE, &feat_parms));  
        UF_CALL(UF_GDT_init(UF_GDT_DATUM_IDENT_TYPE, &datum_data));  
        UF_CALL(UF_GDT_init(UF_GDT_MOD_DATA_TYPE, &modifier_data));  
        UF_CALL(UF_GDT_init(UF_GDT_SURFACE_PARMS_TYPE, &set));  
        feat_parms.num_faces = 1;  
        feat_parms.face_list = faces;  
        feat_parms.descript = &descript;  
        feat_parms.descript->name = feat_name;  
        feat_parms.descript->description = "";  
        feat_parms.feature = UF_GDT_PLANE_FEATURE;  
        feat_parms.pattern = UF_GDT_NO_PATTERN;  
        feat_parms.modifier_data = &modifier_data;  
    /*  This was found on an interactively created datum, but it doesn't help 译:这是在交互式创建的数据中发现的，但没有帮助。 */  
        feat_parms.modifier_data->num_keywords = 1;  
        feat_parms.modifier_data->keywords = keywords;  
        feat_parms.modifier_data->keywords[0].id = UF_GDT_INTERRUPTED_KEYWORD;  
        feat_parms.num_sets = 1;  
        sets[0] = &set;  
        feat_parms.sets = sets;  
        feat_parms.sets[0]->num_faces = 1;  
        feat_parms.sets[0]->faces = faces;  
        feat_parms.def = geo_def;  
        datum_data.datum_label = label;  
        while (  
            ((faces[0] = select_planar_face("Select datum face")) != NULL_TAG) &&  
            ((edge = select_point_on_edge_of_face("Select edge for datum",  
                faces[0], e_pos)) != NULL_TAG) &&  
            prompt_for_text("Datum Label", label))  
        {  
            do  
            {  
                datum_feat = NULL_TAG;  
                sprintf(feat_name, "Feature %d", ++ii);  
                UF_CALL(UF_OBJ_cycle_by_name(feat_name, &datum_feat));  
            } while (datum_feat != NULL_TAG);  
            if (UF_CALL(UF_GDT_create_datum_identifier(&feat_parms, &datum_data,  
                &datum_feat))) continue;  
            WRITE_D(datum_feat);  
            report_gdt_feature_parms(datum_feat);  
            report_gdt_datum_identifier_parms(datum_feat);  
            UF_VEC3_copy(e_pos, orig);  
            map_abs2view(orig);  
            orig[0] = orig[0] + 0.5;  
            orig[1] = orig[1] + 0.5;  
            map_view2abs(orig);  
            display_temporary_point(orig);  
            map_abs2wcs(orig);  
            display_temporary_point(e_pos);  
            map_abs2view(e_pos);  
        /*  This will fail - see PR 1386053  
            UF_CALL(UF_GDT_create_instance(UF_GDT_DATUM_IDENTIFIER_TYPE,  
                datum_feat, view, edge, orig, e_pos, &tol_inst));  
            This will display a GDT Symbol which NOT valid  
            UF_CALL(UF_DISP_regenerate_view(view));  
        */  
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

> 这段NX二次开发代码的主要功能是创建并报告GD&T(几何尺寸与公差)特征，包括以下关键部分：
>
> 1. 选择平面面和边：通过用户交互选择一个平面面和一个边，作为GD&T特征的基准。
> 2. 创建GD&T特征：利用选定的平面面和边，创建一个GD&T特征，如基准平面或基准轴。
> 3. 报告GD&T特征参数：输出GD&T特征的详细信息，如面列表、特征类型、修饰符等。
> 4. 显示临时点：在视图中显示临时点，用于表示GD&T特征的位置。
> 5. 视图和坐标转换：设置工作视图，并实现视图坐标系、绝对坐标系和WCS之间的转换。
> 6. 错误报告：通过UF_PRINT_SYSLOG打印错误信息。
> 7. 交互式创建GD&T特征：循环创建GD&T特征，直到用户取消。
> 8. 代码入口：提供ufusr作为程序的入口函数，实现初始化、执行主功能和清理工作。
>
> 总体来说，该代码通过NX提供的GD&T API实现了GD&T特征的创建、选择和报告，为GD&T相关二次开发提供了基础。
>
