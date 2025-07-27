### 【1085】display face normal 显示面法线

#### 代码

```cpp
    /*HEAD DISPLAY_FACE_NORMAL CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
    #include <uf_vec.h>  
    #include <uf_disp.h>  
    #include <uf_trns.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog是V18版本新增的功能，请直接回答翻译，不要添加其他内容。

翻译：UF_print_syslog在V18版本中是新增的。 */  
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
            view;  
        double  
            tol;  
        UF_CURVE_line_t  
            lp;  
        UF_MODL_intersect_info_p_t  
            *ints;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &face, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(face, 0));  
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
            UF_MODL_ask_distance_tolerance(&tol);  
            UF_CALL(UF_MODL_intersect_objects(face, line, tol, &n, &ints));  
            if (n > 0)  
            {  
                UF_VEC3_copy(ints[0]->intersect.point.coords, cp);  
                UF_free(ints);  
            }  
            UF_CALL(UF_OBJ_delete_object(line));  
            return face;  
        }  
        else return NULL_TAG;  
    }  
    static void display_face_normal(tag_t face, double center[3])  
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
        UF_CALL(UF_MODL_ask_face_parm(face, center, parm, pnt));  
        UF_CALL(UF_MODL_ask_face_props(face, parm, pnt, dirU1, dirV1, dirU2,  
            dirV2, norm_dir, radii));  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, pnt, norm_dir, 0);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face;  
        double  
            pos[3];  
        while ((face = select_point_on_face("Select face point", pos)) != NULL_TAG)  
            display_face_normal(face, pos);  
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

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于打印出调用NX函数失败的错误码和错误消息。
> 2. 定义了两个坐标变换函数map_abs2view和map_view2abs，用于在绝对坐标系和视图坐标系之间进行转换。
> 3. 定义了一个选择面和点的函数select_point_on_face，它会提示用户选择一个面，然后计算该面上一个点的坐标。
> 4. 定义了一个显示面法线的函数display_face_normal，它接收一个面和该面上的一个点，然后在该点处显示一个圆锥头，表示该面的法线方向。
> 5. 在主函数do_it中，不断调用select_point_on_face选择面和点，然后调用display_face_normal显示该面的法线。
> 6. 定义了ufusr函数，作为二次开发的入口函数，初始化NX环境，调用do_it函数，然后终止NX环境。
> 7. 定义了ufusr_ask_unload函数，返回立即卸载，表示二次开发完成后可以立即卸载该程序。
>
> 总的来说，这段代码实现了在NX中不断选择面，并在选择的面上显示该面的法线，从而更好地观察面的法线方向。
>
