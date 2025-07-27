### 【3088】create rectangles 在UG中创建矩形的用户函数示例代码

#### 代码

```cpp
    /*HEAD CREATE_RECTANGLES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_disp.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，翻译结果如下：

UF_print_syslog 是 V18 新增的函数。 */  
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
    static void ask_wcs_x_and_y_axes(double *xaxis, double *yaxis)  
    {  
        tag_t  
            wcs,  
            wcs_mx;  
        double  
            axes[9],  
            mag,  
            origin[3];  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs, &wcs_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, axes));  
        UF_VEC3_unitize(&axes[0], 0, &mag, xaxis);  
        UF_VEC3_unitize(&axes[3], 0, &mag, yaxis);  
    }  
    struct rectangle_data_s {  
        double  
            origin[3],  
            x_axis[3],  
            y_axis[3],  
            opposite[3];  
    };  
    typedef struct rectangle_data_s rectangle_data_t, *rectangle_data_p_t;  
    /*  This motion callback emulates the rubberbanding rectangle seen when  
        using the interactive option Insert->Curve->Rectangle...  */  
    static void rubberband_rectangle_cb(double *screen_pos,  
        UF_UI_motion_cb_data_p_t motion_cb_data, void *client_data)  
    {  
        double  
            dx[3],  
            dy[3],  
            end[3],  
            pos[5][3],  
            start[3];  
        rectangle_data_p_t  
            my_data = (rectangle_data_p_t) client_data;  
        UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, my_data->origin,  
            UF_CSYS_ROOT_WCS_COORDS, start);  
        UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, screen_pos,  
            UF_CSYS_ROOT_WCS_COORDS, end);  
        UF_VEC3_scale( end[0] - start[0], my_data->x_axis, dx );  
        UF_VEC3_scale( end[1] - start[1], my_data->y_axis, dy );  
        UF_VEC3_copy( my_data->origin, pos[0] );  
        UF_VEC3_add( pos[0], dx, pos[1] );  
        UF_VEC3_add( pos[1], dy, pos[2] );  
        UF_VEC3_sub( pos[2], dx, pos[3] );  
        UF_VEC3_sub( pos[3], dy, pos[4] );  
        UF_CALL(UF_DISP_display_ogp_polyline(motion_cb_data->view_tag, pos, 5));  
    }  
    static logical specify_rectangle(char *prompt, rectangle_data_p_t the_data)  
    {  
        int  
            resp;  
        tag_t  
            view;  
        char  
            msg[133];  
        sprintf(msg, "%s corner 1", prompt);  
        UF_CALL(UF_UI_specify_screen_position(msg, NULL, NULL, the_data->origin,  
            &view, &resp));  
        if (resp != UF_UI_PICK_RESPONSE) return FALSE;  
        ask_wcs_x_and_y_axes(the_data->x_axis, the_data->y_axis);  
        sprintf(msg, "%s corner 2", prompt);  
        UF_CALL(UF_UI_specify_screen_position(msg, rubberband_rectangle_cb,  
            (void *)the_data, the_data->opposite, &view, &resp));  
        if (resp != UF_UI_PICK_RESPONSE) return FALSE;  
        return TRUE;  
    }  
    static void create_curve_rectangle(rectangle_data_p_t where)  
    {  
        tag_t  
            lines[4];  
        double  
            pos[4][3],  
            dx[3],  
            dy[3],  
            end[3],  
            start[3];  
        UF_CURVE_line_t  
            line_data = { 0,0,0, 0,0,0 };  
        UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, where->origin,  
            UF_CSYS_ROOT_WCS_COORDS, start);  
        UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, where->opposite,  
            UF_CSYS_ROOT_WCS_COORDS, end);  
        UF_VEC3_scale( end[0] - start[0], where->x_axis, dx );  
        UF_VEC3_scale( end[1] - start[1], where->y_axis, dy );  
        UF_VEC3_copy( where->origin, pos[0] );  
        UF_VEC3_add( pos[0], dx, pos[1] );  
        UF_VEC3_add( pos[1], dy, pos[2] );  
        UF_VEC3_sub( pos[2], dx, pos[3] );  
        UF_VEC3_copy( pos[0], line_data.start_point);  
        UF_VEC3_copy( pos[1], line_data.end_point);  
        UF_CALL(UF_CURVE_create_line(&line_data, &lines[0]));  
        UF_VEC3_copy( pos[1], line_data.start_point);  
        UF_VEC3_copy( pos[2], line_data.end_point);  
        UF_CALL(UF_CURVE_create_line(&line_data, &lines[1]));  
        UF_VEC3_copy( pos[2], line_data.start_point);  
        UF_VEC3_copy( pos[3], line_data.end_point);  
        UF_CALL(UF_CURVE_create_line(&line_data, &lines[2]));  
        UF_VEC3_copy( pos[3], line_data.start_point);  
        UF_VEC3_copy( pos[0], line_data.end_point);  
        UF_CALL(UF_CURVE_create_line(&line_data, &lines[3]));  
    }  
    static void do_it(void)  
    {  
        rectangle_data_t  
            where;  
        while (specify_rectangle("Define rectangle", &where))  
            create_curve_rectangle(&where);  
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

> 根据代码注释和结构，这是一段用于在NX中创建矩形的二次开发代码。代码的主要功能和流程如下：
>
> 1. 头文件和宏定义：引入NX API相关的头文件，并定义了UF_CALL宏用于调用NX API并捕获错误。
> 2. 错误报告函数：report_error用于报告调用NX API时的错误信息。
> 3. 获取坐标系函数：ask_wcs_x_and_y_axes用于获取当前工作坐标系(WCS)的X轴和Y轴单位向量。
> 4. 矩形数据结构：rectangle_data_t用于存储矩形的原点、X轴、Y轴和对角点坐标。
> 5. 绘制矩形回调函数：rubberband_rectangle_cb用于在交互式绘制矩形时实时显示矩形框。
> 6. 获取矩形顶点函数：specify_rectangle用于获取用户指定的矩形两个顶点，并计算出矩形的X轴和Y轴。
> 7. 创建矩形曲线函数：create_curve_rectangle根据矩形数据结构中的信息，创建构成矩形的四条直线。
> 8. 主函数：do_it用于循环调用specify_rectangle获取用户指定的矩形，然后调用create_curve_rectangle创建矩形。
> 9. NX启动和退出函数：ufusr用于在NX启动时初始化NX API，并调用do_it函数，在NX退出时终止NX API。
> 10. 卸载函数：ufusr_ask_unload用于卸载NX API。
>
> 总的来说，这段代码实现了在NX中通过交互式方式绘制矩形的功能，为用户提供了一个创建矩形的二次开发示例。
>
