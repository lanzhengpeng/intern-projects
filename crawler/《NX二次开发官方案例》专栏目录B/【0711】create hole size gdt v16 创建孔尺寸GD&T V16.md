### 【0711】create hole size gdt v16 创建孔尺寸GD&T V16

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
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本新增的函数。因此，翻译内容只需简洁地表达这一信息即可，无需添加任何其他内容。 */  
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
    static logical work_around_1(tag_t *object)  
    {  
        int  
            d_units,  
            p_units;  
        tag_t  
            comp,  
            part;  
        uf_list_p_t  
            face_list;  
        if (UF_ASSEM_is_occurrence(*object))  
        {  
            comp = UF_ASSEM_ask_part_occurrence(*object);  
            part = UF_ASSEM_ask_prototype_of_occ(comp);  
            UF_CALL(UF_PART_ask_units(part, &p_units));  
            UF_CALL(UF_PART_ask_units(UF_PART_ask_display_part(), &d_units));  
            if (p_units != d_units)  
            {  
                UF_CALL(UF_MODL_extract_face(*object, 0, object));  
                UF_CALL(UF_MODL_ask_body_faces(*object, &face_list));  
                UF_CALL(UF_MODL_ask_list_item(face_list, 0, object));  
                UF_CALL(UF_MODL_delete_list(&face_list));  
                return TRUE;  
            }  
        }  
        return FALSE;  
    }  
    static tag_t select_point_on_cylindrical_face(char *prompt, double cp[3])  
    {  
        logical  
            fix;  
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
            fix = work_around_1(&face);  
            UF_MODL_ask_distance_tolerance(&tol);  
            UF_CALL(UF_MODL_intersect_objects(face, line, tol, &n, &ints));  
            if (n > 0)  
            {  
                UF_VEC3_copy(ints[0]->intersect.point.coords, cp);  
                UF_free(ints);  
            }  
            UF_CALL(UF_OBJ_delete_object(line));  
            if (fix) UF_CALL(UF_OBJ_delete_object(face));  
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
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE };  
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
            matobj,                     /* Matrix object 译:翻译Matrix object为“矩阵对象”。 */  
            csys;                       /* Csys object  译:Csys 对象 */  
        double  
            origin[3],                  /* View's center 译:视图的中心 */  
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
    static void do_it(void)  
    {  
        int  
            ii = 0;  
        tag_t  
            edge,  
            face,  
            orig_wcs,  
            size_tol,  
            tol_inst;  
        double  
            e_pos[3],  
            f_pos[3],  
            orig[3];  
        char  
            feat_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_GDT_feature_parms_t  
            feat_parms;  
        UF_GDT_size_tolerance_t  
            size_data;  
        UF_GDT_description_t  
            descript;  
        UF_CALL(UF_CSYS_ask_wcs(&orig_wcs));  
        set_wcs_to_view("");  
        descript.name = feat_name;  
        descript.description = "Hole Size Feature";  
        feat_parms.num_faces = 1;  
        feat_parms.face_list = &face;  
        feat_parms.descript = &descript;  
        feat_parms.feature = UF_GDT_HOLE_FEATURE;  
        feat_parms.pattern = UF_GDT_NO_PATTERN;  
        feat_parms.modifier_data = NULL;  
    /*  
        size_data.value_type = UF_GDT_DIAMETRAL_SIZE;  
    */  
        size_data.dimension = NULL_TAG;  
        UF_CALL(UF_MODL_create_exp_tag("0.25", &size_data.upper_tol.expression));  
        size_data.upper_tol.value = 0.25;  
        size_data.upper_tol.decimal_format = 2;  
        UF_CALL(UF_MODL_create_exp_tag("-.25", &size_data.lower_tol.expression));  
        size_data.lower_tol.value = -.25;  
        size_data.lower_tol.decimal_format = 2;  
        size_data.tol_format = UF_GDT_PLUS_OVER_MINUS;  
        size_data.modifier_data = NULL;  
    /*  
        size_data.decimal_format = 2;  
    */  
        while (  
            ((face = select_point_on_cylindrical_face("Select hole face", f_pos))  
                != NULL_TAG) &&  
            ((edge = select_point_on_edge("Select edge for tolerance", e_pos))  
                != NULL_TAG))  
        {  
            size_data.nominal_value = ask_cylindrical_face_diameter(face, f_pos);  
            do  
            {  
                size_tol = NULL_TAG;  
                sprintf(feat_name, "HoleFeature%d", ++ii);  
                UF_CALL(UF_OBJ_cycle_by_name(feat_name, &size_tol));  
            } while (size_tol != NULL_TAG);  
            UF_CALL(UF_GDT_create_size_tolerance(&feat_parms, &size_data,  
                &size_tol));  
            UF_VEC3_copy(e_pos, orig);  
            map_abs2view(orig);  
            orig[0] = orig[0] + 0.5;  
            orig[1] = orig[1] + 0.5;  
            map_view2abs(orig);  
            display_temporary_point(orig);  
            map_abs2wcs(orig);  
            display_temporary_point(e_pos);  
            map_abs2view(e_pos);  
            UF_CALL(UF_GDT_create_instance(UF_GDT_SIZE_TOLERANCE_TYPE, size_tol,  
                NULL_TAG, edge, orig, e_pos, &tol_inst));  
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

> 这是段NX二次开发代码，主要实现了以下功能：
>
> 1. 错误报告：定义了一个错误报告函数，用于在出错时输出错误信息。
> 2. 坐标系转换：实现了绝对坐标系到视图坐标系、视图坐标系到绝对坐标系、绝对坐标系到工作坐标系的转换函数。
> 3. 选择和定位：实现了选择圆柱面和边线，并获取其位置和尺寸的函数。
> 4. 视图设置：实现了设置视图为当前工作坐标系的函数。
> 5. 显示临时点：实现了显示临时点的函数。
> 6. 创建尺寸公差：实现了创建尺寸公差的函数，并能够实例化到边线上。
> 7. NX初始化和终止：实现了NX的初始化和终止函数。
> 8. 请求卸载：实现了请求立即卸载的函数。
>
> 总的来说，这段代码实现了在NX中创建尺寸公差并实例化到边线上的功能。
>
