### 【2816】set wcs at selected face point using U and normal 在选定面点使用U和法向量设置WCS

#### 代码

```cpp
    /*HEAD SET_WCS_AT_SELECTED_FACE_POINT_USING_U_AND_NORMAL CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
    #include <uf_vec.h>  
    #include <uf_disp.h>  
    #include <uf_trns.h>  
    #include <uf_view.h>  
    #include <uf_mtx.h>  
    #include <uf_csys.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中的新功能。 */  
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
    /* qq3123197280 */  
    static int mask_for_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_point_on_face(char *prompt, double cp[3])  
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
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
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
    static void set_wcs_by_face_u_and_normal_at_point(tag_t face, double origin[3])  
    {  
        tag_t  
            mx,  
            wcs;  
        double  
            csys[9],  
            junk[3],  
            mag,  
            parm[2],  
            pnt[3],  
            tol,  
            x_dir[3],  
            y_dir[3],  
            z_dir[3];  
        UF_MODL_ask_distance_tolerance(&tol);  
        UF_CALL(UF_MODL_ask_face_parm(face, origin, parm, pnt));  
        UF_CALL(UF_MODL_ask_face_props(face, parm, pnt, x_dir, junk, junk,  
            junk, z_dir, junk));  
        UF_VEC3_cross(z_dir, x_dir, y_dir);  
        UF_CALL(UF_VEC3_unitize(x_dir, tol, &mag, &csys[0]));  
        UF_CALL(UF_VEC3_unitize(y_dir, tol, &mag, &csys[3]));  
        UF_CALL(UF_VEC3_unitize(z_dir, tol, &mag, &csys[6]));  
        UF_MTX3_ortho_normalize(csys);  
        UF_CALL(UF_CSYS_create_matrix(csys, &mx));  
        UF_CALL(UF_CSYS_create_temp_csys(origin, mx, &wcs));  
        UF_CALL(UF_CSYS_set_wcs(wcs));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face;  
        double  
            pos[3];  
        while ((face = select_point_on_face("Select face", pos)) != NULL_TAG)  
            set_wcs_by_face_u_and_normal_at_point(face, pos);  
    }  
    /* qq3123197280 */  
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

> 这段代码是一个NX二次开发用户函数，其主要功能是根据用户在面上选择的一个点，将工作坐标系(WCS)设置为该点并使用面的U方向和法向。
>
> 主要步骤如下：
>
> 1. 包含了NX Open API的头文件，用于进行二次开发。
> 2. 定义了错误报告函数report_error，用于打印出错误信息。
> 3. 定义了两个坐标系转换函数map_abs2view和map_view2abs，用于将绝对坐标系坐标转换为视图坐标系坐标，反之亦然。
> 4. 定义了选择面的函数mask_for_faces，用于设置选择模式以选择面。
> 5. 定义了选择面上的点的函数select_point_on_face，使用单选对话框让用户选择面，并获取用户选择的点。
> 6. 定义了根据面和点设置WCS的函数set_wcs_by_face_u_and_normal_at_point，首先根据面参数和点坐标计算出面的U方向和法向，然后创建一个临时坐标系，并将其设置为WCS。
> 7. 定义了主函数do_it，循环调用select_point_on_face和set_wcs_by_face_u_and_normal_at_point，直到用户选择退出。
> 8. 定义了ufusr函数，用于初始化NX Open API、调用主函数do_it，并终止NX Open API。
> 9. 定义了ufusr_ask_unload函数，用于卸载用户函数。
>
> 总体而言，这段代码通过选择一个面上的点，将WCS设置为该点并使用面的U方向和法向，实现了坐标系与模型几何关联的功能。
>
