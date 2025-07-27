### 【0702】create fitted spline 创建拟合样条线

#### 代码

```cpp
    /*HEAD CREATE_FITTED_SPLINE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：请注意，UF_print_syslog 是 V18 中的新增功能，请只提供译文，不要添加其他无关内容。 */  
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
            ii,  
            jj,  
            max_err_pt,  
            resp;  
        tag_t  
            spline;  
        double  
            max_err;  
        UF_UI_chained_points_t  
            *points;  
        SPLINE_FIT_t  
            spline_data;  
        spline_data.slopes = NULL;  
        spline_data.weights = NULL;  
        spline_data.tolerance = 1.0;  
        spline_data.slope_flag = 0;  
        spline_data.num_of_weights = 0;  
        spline_data.weight_positions = NULL;  
        spline_data.num_of_segments = 0;  
        spline_data.degree = 3;  
        while ( !UF_CALL(UF_UI_select_point_collection("Select points", FALSE,  
               &points, &spline_data.num_of_points, &resp)) && (resp == UF_UI_OK))  
        {  
            spline_data.points = malloc(3*spline_data.num_of_points*sizeof(double));  
            for (ii = 0; ii < spline_data.num_of_points; ii++)  
                for (jj = 0; jj < 3; jj++)  
                    spline_data.points[(ii * 3) + jj] = points[ii].pt[jj];  
            UF_CALL(UF_MODL_create_fitted_spline(&spline_data, &max_err,  
                &max_err_pt, &spline));  
            free(spline_data.points);  
            UF_free(points);  
        }  
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

> 这是NX二次开发的示例代码，主要实现了以下功能：
>
> 1. 定义了错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 2. 定义了do_it函数，用于创建拟合样条。该函数首先通过UI选择点集，然后为点集分配内存，并调用UF_MODL_create_fitted_spline创建拟合样条。
> 3. 定义了ufusr主函数，用于初始化和调用do_it函数。
> 4. 定义了ufusr_ask_unload函数，用于在卸载时返回立即卸载标志。
> 5. 使用了UF_CALL宏来包装UF函数调用，并在失败时调用report_error报告错误。
> 6. 在创建拟合样条时，指定了样条曲线的度数为3，并初始化了其他相关参数。
> 7. 在选择点集成功后，为点集分配内存，并将点集数据复制到spline_data结构中。
> 8. 调用UF_MODL_create_fitted_spline创建拟合样条，并释放内存。
> 9. 在函数内部处理了点集选择和创建拟合样条的交互。
> 10. 通过UF_print_syslog打印了日志信息。
>
> 总体来看，这段代码实现了通过UI选择点集并创建拟合样条的功能，同时包含了错误处理、内存管理等辅助功能。
>
