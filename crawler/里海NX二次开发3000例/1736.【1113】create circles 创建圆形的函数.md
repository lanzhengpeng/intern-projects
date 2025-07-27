### 【1113】create circles 创建圆形的函数

#### 代码

```cpp
    /*HEAD CREATE_CIRCLES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_disp.h>  
    #include <uf_vec.h>  
    #include <uf_csys.h>  
    #include <uf_mtx.h>  
    #include <uf_trns.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是一个在V18版本中新增的功能。文档要求只提供翻译，不添加任何额外内容。

翻译结果为：UF_print_syslog是V18版本新增的功能。 */  
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
    static tag_t ask_wcs_matrix_tag(void)  
    {  
        tag_t  
            wcs,  
            wcs_mx;  
        double  
            origin[3];  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs, &wcs_mx, origin));  
        return wcs_mx;  
    }  
    static void map_arc_center_from_abs(UF_CURVE_arc_t *adata)  
    {  
        int  
            irc;  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12] = { 0,0,0,0,0,0,0,0,0,0,0,0},  
            mx[12];  
        UF_CALL(UF_CSYS_ask_matrix_values(adata->matrix_tag, &csys[3]));  
        FTN(uf5940)(abs_mx, csys, mx, &irc);  
        FTN(uf5941)(adata->arc_center, mx);  
    }  
    static void map_point_to_matrix(tag_t matrix, double *pos)  
    {  
        int  
            irc;  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12] = { 0,0,0,0,0,0,0,0,0,0,0,0},  
            mx[12];  
        UF_CALL(UF_CSYS_ask_matrix_values(matrix, &csys[3]));  
        FTN(uf5940)(abs_mx, csys, mx, &irc);  
        FTN(uf5941)(pos, mx);  
    }  
    /*  This motion callback emulates the rubberbanding circle seen when  
        using the interactive option Insert->Curve->Basic Curves...->Circle  */  
    static void rubberband_circle_cb(double *screen_pos,  
        UF_UI_motion_cb_data_p_t motion_cb_data, void *client_data)  
    {  
        double  
            axes[9];  
        UF_CURVE_arc_p_t  
            my_data = (UF_CURVE_arc_p_t) client_data;  
        map_point_to_matrix(my_data->matrix_tag, screen_pos);  
        UF_VEC3_distance(my_data->arc_center, screen_pos, &my_data->radius);  
        if (my_data->radius > 0)  
        {  
            UF_CALL(UF_CSYS_ask_matrix_values(my_data->matrix_tag, axes));  
            UF_CALL(UF_DISP_display_ogp_circle(motion_cb_data->view_tag,  
                axes, my_data->arc_center, my_data->radius));  
        }  
    }  
    static logical specify_circle(char *prompt, UF_CURVE_arc_p_t the_data)  
    {  
        int  
            resp;  
        tag_t  
            view;  
        char  
            msg[133];  
        double  
            on_arc[3];  
        sprintf(msg, "%s center", prompt);  
        UF_CALL(UF_UI_specify_screen_position(msg, NULL, NULL,  
            the_data->arc_center, &view, &resp));  
        if (resp != UF_UI_PICK_RESPONSE) return FALSE;  
        the_data->matrix_tag = ask_wcs_matrix_tag();  
        map_arc_center_from_abs(the_data);  
        sprintf(msg, "%s point on arc", prompt);  
        UF_CALL(UF_UI_specify_screen_position(msg, rubberband_circle_cb,  
            (void *)the_data, on_arc, &view, &resp));  
        if (resp != UF_UI_PICK_RESPONSE) return FALSE;  
        map_point_to_matrix(the_data->matrix_tag, on_arc);  
        UF_VEC3_distance(the_data->arc_center, on_arc, &the_data->radius);  
        return TRUE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            arc;  
        UF_CURVE_arc_t  
            arc_data = { NULL_TAG, 0, TWOPI, { 0,0,0 }, 1.0 };  
        while (specify_circle("Define circle", &arc_data))  
            UF_CALL(UF_CURVE_create_arc(&arc_data, &arc));  
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

> 这段代码是NX的二次开发代码，实现了在NX中创建圆弧的功能。主要功能包括：
>
> 1. 使用UF_CSYS接口获取当前工作坐标系(WCS)的矩阵标签。
> 2. 使用UF_CURVE接口创建圆弧。
> 3. 使用UF_DISP接口显示临时圆弧。
> 4. 使用UF_VEC接口计算两点之间的距离。
> 5. 使用UF_UI接口获取用户指定的屏幕位置。
> 6. 使用UF_TRNS接口转换点坐标。
> 7. 使用UF_print_syslog接口打印错误信息到系统日志。
> 8. 实现了一个回调函数，用于在指定圆弧中心后，动态显示临时圆弧。
> 9. 实现了一个函数，用于获取用户指定的圆弧中心和半径，并创建圆弧。
> 10. 实现了UFusr函数，初始化NX环境，调用创建圆弧功能，然后终止NX环境。
> 11. 实现了卸载函数，用于在卸载时立即终止NX环境。
>
> 总的来说，这段代码通过NX的二次开发接口，实现了交互式创建圆弧的功能。
>
