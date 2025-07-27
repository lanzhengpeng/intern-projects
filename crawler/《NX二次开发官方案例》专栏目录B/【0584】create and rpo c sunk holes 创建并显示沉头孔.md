### 【0584】create and rpo c sunk holes 创建并显示沉头孔

#### 代码

```cpp
    /*HEAD CREATE_AND_RPO_C_SUNK_HOLES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_trns.h>  
    #include <uf_view.h>  
    #include <uf_vec.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本中的新功能。 */  
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
    static void get_face_dirs(double loc[3], tag_t face, double xyz[9])  
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
            get_face_dirs(loc, face, dest_csys);  
            UF_VEC3_negate(&dest_csys[6], dir);  
            UF_CALL(UF_MODL_create_c_sunk_hole(loc, dir, "0.25", "1.5", "0.5",  
                "82", "118", face, NULL_TAG, &hole));  
        }  
        UF_CALL(UF_MODL_unregister_rpo_routine());  
    }  
    /* 里海 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
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

> 这段代码是一个NX二次开发示例，其主要功能是在NX的部件上创建沉头孔。以下是代码的主要内容和功能：
>
> 1. 包含了NX提供的UF函数库，如uf.h、uf_part.h等，用于调用NX的API函数。
> 2. 定义了一个错误报告函数report_error，用于输出UF函数调用错误信息。
> 3. 定义了两个坐标系映射函数map_abs2view和map_view2abs，用于将点在绝对坐标系和视图坐标系之间进行转换。
> 4. 定义了一个选择面和点的函数select_point_on_face_in_work_part，通过对话框提示用户选择一个面，并返回面和面上一个点的信息。
> 5. 定义了一个获取面方向向量的函数get_face_dirs，用于计算一个面上某点的U方向和V方向向量。
> 6. 定义了主函数do_it，其中注册了沉头孔创建的RPO菜单，然后循环提示用户选择面，并在面上创建沉头孔。
> 7. 定义了UFUSR入口函数ufusr，初始化NX环境，调用do_it函数，然后结束NX环境。
> 8. 定义了卸载函数ufusr_ask_unload，指示该例程不能被卸载。
>
> 综上所述，这段代码通过交互方式提示用户选择面，并在面上创建沉头孔，展示了NX二次开发的基本流程。
>
