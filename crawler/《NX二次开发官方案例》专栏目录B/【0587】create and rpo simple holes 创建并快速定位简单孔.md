### 【0587】create and rpo simple holes 创建并快速定位简单孔

#### 代码

```cpp
    /*HEAD CREATE_AND_RPO_SIMPLE_HOLES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
    #include <uf_assem.h>  
    #include <uf_vec.h>  
    #include <uf_trns.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本中新增的函数。 */  
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
    static void map_abs2view(double c[3], tag_t view)  
    {  
        char  
            vname[30];  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(UF_OBJ_ask_name(view, vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(abs, vw, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void map_view2abs(double c[3], tag_t view)  
    {  
        char  
            vname[30];  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(UF_OBJ_ask_name(view, vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(vw, abs, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    /* 里海 */  
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
    static tag_t select_point_on_face_in_work_part(char *prompt, double cp[3])  
    {  
        int  
            n,  
            resp;  
        tag_t  
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
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            UF_CALL(UF_CSYS_map_point(UF_CSYS_WORK_COORDS, cp, UF_CSYS_ROOT_COORDS,  
                cp));  
            map_abs2view(cp, view);  
            lp.start_point[0] = cp[0];  
            lp.start_point[1] = cp[1];  
            lp.start_point[2] = cp[2] + 1000;  
            lp.end_point[0] = cp[0];  
            lp.end_point[1] = cp[1];  
            lp.end_point[2] = cp[2] - 1000;  
            map_view2abs(lp.start_point, view);  
            map_view2abs(lp.end_point, view);  
            UF_CALL(UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, lp.start_point,  
                UF_CSYS_WORK_COORDS, lp.start_point));  
            UF_CALL(UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, lp.end_point,  
                UF_CSYS_WORK_COORDS, lp.end_point));  
            UF_CALL(UF_CURVE_create_line(&lp, &line));  
            UF_MODL_ask_distance_tolerance(&tol);  
            UF_CALL(UF_MODL_intersect_objects(object, line, tol, &n, &ints));  
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
    static void ask_face_dirs(double loc[3], tag_t face, double xyz[9])  
    {  
        double  
            parm[2],  
            pnt[3],  
            dirU2[3],  
            dirV2[3],  
            radii[2];  
        UF_CALL(UF_MODL_ask_face_parm(face, loc, parm, pnt));  
        UF_CALL(UF_MODL_ask_face_props(face, parm, pnt, &xyz[0], &xyz[3], dirU2,  
            dirV2, &xyz[6], radii));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face,  
            hole;  
        double  
            dir[3],  
            loc[3],  
            dest_csys[9];  
        UF_CALL(UF_MODL_register_rpo_routine(UF_MODL_default_rpo_menu));  
        while ((face = select_point_on_face_in_work_part("Select face for hole",  
                loc)) != NULL_TAG)  
        {  
            ask_face_dirs(loc, face, dest_csys);  
            UF_VEC3_negate(&dest_csys[6], dir);  
            UF_CALL(UF_MODL_create_simple_hole(loc, dir, "0.5", "0.5", "118",  
                face, NULL_TAG, &hole));  
        }  
        UF_CALL(UF_MODL_unregister_rpo_routine());  
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
    /*  Note:  can NOT be unloaded - calls UF_MODL_register_rpo_routine.  
        return (UF_UNLOAD_IMMEDIATELY);  
    */  
        return (UF_UNLOAD_UG_TERMINATE);  
    }

```

#### 代码解析

> 这段代码是一个NX Open的UF程序，用于在用户选择的面上创建一个简单的孔。以下是代码的主要功能和流程：
>
> 1. 包含了必要的头文件，包括UF对象、显示、曲线、建模、装配、向量、转换和视图等UF模块的头文件。
> 2. 定义了UF_CALL宏，用于简化UF函数的调用，并能够报告错误。
> 3. 定义了两个坐标转换函数，用于将绝对坐标系下的点转换到视图坐标系下，反之亦然。
> 4. 定义了mask_for_faces函数，用于设置选择器，使其仅选择面。
> 5. 定义了select_point_on_face_in_work_part函数，用于在当前工作部件中弹出一个对话框，让用户选择一个面，并返回面的tag。
> 6. 定义了ask_face_dirs函数，用于获取指定面上某点的法向量和两个主方向向量。
> 7. 定义了do_it函数，用于注册RPO菜单，循环让用户选择面，在面上创建孔。
> 8. ufusr是程序的入口函数，初始化后调用do_it，完成后终止。
> 9. ufusr_ask_unload函数声明了程序不能被卸载，因为调用了UF_MODL_register_rpo_routine。
>
> 总的来说，这段代码实现了在用户选择的面上创建简单孔的功能，使用了UF建模模块提供的函数，并通过对话框与用户交互选择面。
>
