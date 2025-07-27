### 【0981】create smart bcurve thru points 创建通过点的智能B样条

#### 代码

```cpp
    /*HEAD CREATE_SMART_BCURVE_THRU_POINTS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_curve.h>  
    #include <uf_so.h>  
    #include <uf_view.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog 是 V18 新增的。 */  
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
    static void do_it(void)  
    {  
        int  
            num_points = 5,  
            degree = 3,  
            non_periodic = 0,     /* 1 would produce a closed spline 译:产生一个闭合的样条线。 */  
            start_slope_type = 0, /* auto slope 译:自动斜率 */  
            end_slope_type = 0;   /* auto slope 译:自动坡度 */  
        tag_t  
            pts[5],  
            smart_curve = NULL_TAG,  
            start_slope = NULL_TAG,  
            end_slope = NULL_TAG;  
        double    
            ptc[3] = { 0., 0., 0. };  
        double   
            * point_parameters = NULL;   
        const double   
            dbl = 0.0;  
        UF_CALL(UF_CURVE_create_point( ptc, &pts[0] ));  
        ptc[0] = 2.0;  
        ptc[1] = 2.0;      
        UF_CALL(UF_CURVE_create_point( ptc, &pts[1] ));  
        ptc[0] = 4.0;  
        ptc[1] = 4.0;      
        UF_CALL(UF_CURVE_create_point( ptc, &pts[2] ));  
        ptc[0] = 6.0;  
        ptc[1] = 2.0;      
        UF_CALL(UF_CURVE_create_point( ptc, &pts[3] ));  
        ptc[0] = 8.0;  
        ptc[1] = 0.0;      
        UF_CALL(UF_CURVE_create_point( ptc, &pts[4] ));  
        UF_VIEW_fit_view( NULL_TAG, 0.8 );  
        UF_CALL(UF_SO_create_scalar_double ( pts[0], UF_SO_update_after_modeling,   
                                             dbl, &start_slope ));  
        UF_CALL(UF_SO_create_scalar_double ( pts[0], UF_SO_update_after_modeling,   
                                             dbl, &end_slope ));  
        UF_CALL(UF_SO_create_bcurve_thru_points( UF_SO_update_after_modeling,   
                                                 num_points, pts,   
                                                 point_parameters, degree,   
                                                 non_periodic, start_slope,  
                                                 end_slope, start_slope_type,  
                                                 end_slope_type, &smart_curve ));  
        UF_CALL(UF_SO_set_visibility_option( smart_curve, UF_SO_visible));  
        return;  
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

> 这段NX二次开发代码主要用于创建一个通过给定点的智能样条曲线。
>
> 主要功能包括：
>
> 1. 定义了错误报告函数report_error，用于在调用NX函数出错时打印错误信息。
> 2. 在do_it函数中，定义了5个点的坐标，并调用UF_CURVE_create_point函数创建这些点。
> 3. 调用UF_VIEW_fit_view函数调整视图，以更好地显示结果。
> 4. 创建起始和终止斜率约束，用于控制样条曲线的起始和终止部分。
> 5. 调用UF_SO_create_bcurve_thru_points函数，通过给定点创建一个智能样条曲线。可以设置曲线的度数、是否闭合等参数。
> 6. 设置样条曲线的可见性，使其在NX中可见。
> 7. 在ufusr函数中，初始化NX环境，调用do_it函数创建样条曲线，然后终止NX环境。
> 8. ufusr_ask_unload函数设置在NX卸载该程序时立即卸载。
>
> 综上所述，这段代码实现了通过点创建智能样条曲线的功能，是NX二次开发中的一个基础示例。
>
