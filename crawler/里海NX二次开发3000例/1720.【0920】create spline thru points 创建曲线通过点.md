### 【0920】create spline thru points 创建曲线通过点

#### 代码

```cpp
    /*HEAD CREATE_SPLINE_THRU_POINTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_obj.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中的新功能。

以下是翻译结果：

注意：UF_print_syslog是V18版本中的新增功能。 */  
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
            ii;  
        tag_t  
            point,  
            spline;  
        double  
            pts[21][3] = { { 1000.00, -928.90, 659.80 },  
                           { 1000.00, -952.60, 701.00 },  
                           { 1000.00, -963.50, 732.00 },  
                           { 1000.00, -963.50, 761.00 },  
                           { 1000.00, -958.10, 785.80 },  
                           { 1000.00, -948.50, 810.70 },  
                           { 1000.00, -938.90, 827.30 },  
                           { 1000.00, -930.40, 836.20 },  
                           { 1000.00, -919.70, 840.10 },  
                           { 1000.00, -872.60, 847.50 },  
                           { 1000.00, -825.50, 850.00 },  
                           { 1000.00, -778.40, 847.50 },  
                           { 1000.00, -731.30, 840.10 },  
                           { 1000.00, -720.60, 836.20 },  
                           { 1000.00, -712.10, 827.30 },  
                           { 1000.00, -702.50, 810.70 },  
                           { 1000.00, -692.90, 785.80 },  
                           { 1000.00, -687.50, 761.00 },  
                           { 1000.00, -687.50, 732.00 },  
                           { 1000.00, -698.40, 701.00 },  
                           { 1000.00, -722.10, 659.80 } };  
        UF_CURVE_pt_slope_crvatr_t  
            point_data[21];  
        for (ii = 0; ii < 21; ii++)  
        {  
            UF_VEC3_copy(pts[ii], point_data[ii].point);  
            if ((ii == 0) || (ii == 20))  
                point_data[ii].slope_type = UF_CURVE_SLOPE_AUTO;  
            else  
                point_data[ii].slope_type = UF_CURVE_SLOPE_NONE;  
            point_data[ii].crvatr_type = UF_CURVE_CRVATR_NONE;  
            UF_CALL(UF_CURVE_create_point(pts[ii], &point));  
            UF_CALL(UF_OBJ_set_color(point, UF_OBJ_YELLOW));  
        }  
        if (!UF_CALL(UF_CURVE_create_spline_thru_pts(3, FALSE, 21, point_data,  
            NULL, FALSE, &spline)))  
            UF_CALL(UF_OBJ_set_color(spline, UF_OBJ_CYAN));  
    }  
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

> 这段代码是用于NX的二次开发，主要实现了通过给定的一系列点创建样条曲线的功能。主要步骤包括：
>
> 1. 定义了点的坐标数组pts，并包含了21个点的三维坐标。
> 2. 定义了点数据的结构体数组point_data，用于存储每个点的位置、斜率类型和曲率类型。对于首尾点，设置自动斜率，内部点设置无斜率；曲率类型全部设置为无。
> 3. 使用UF_CURVE_create_point函数创建每个点，并设置点的颜色为黄色。
> 4. 使用UF_CURVE_create_spline_thru_pts函数通过这些点创建一条样条曲线，设置曲线的颜色为青色。
> 5. 在ufusr函数中，初始化NX，调用do_it函数执行创建样条曲线的操作，然后结束NX。
> 6. ufusr_ask_unload函数用于卸载NX插件时立即卸载。
> 7. 报错处理函数report_error用于捕获NX函数调用错误，并打印错误信息。
>
> 综上所述，这段代码实现了通过给定点创建样条曲线的功能，并提供了错误处理和颜色设置等细节。
>
