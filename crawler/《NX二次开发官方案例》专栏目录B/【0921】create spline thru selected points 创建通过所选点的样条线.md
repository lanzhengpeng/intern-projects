### 【0921】create spline thru selected points 创建通过所选点的样条线

#### 代码

```cpp
    /*HEAD CREATE_SPLINE_THRU_SELECTED_POINTS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 版本中的新功能。 */  
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
    static logical ask_true_or_false(char *prompt, int *deflt)  
    {  
        int  
            resp;  
        char  
            options[2][38] = { "True", "False" };  
        resp = 2 - *deflt;  
        resp = uc1603(prompt, resp, options, 2);  
        if (resp > 4)  
        {  
            *deflt = 6 - resp;  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            err,  
            flag = FALSE,  
            ii,  
            n,  
            resp;  
        tag_t  
            spline;  
        UF_UI_chained_points_t  
            *points;  
        UF_CURVE_pt_slope_crvatr_t  
            *point_data;  
        double  
            *spts;  
        while (ask_true_or_false("Closed curve?", &flag) &&  
               !UF_CALL(UF_UI_select_point_collection("Select points", FALSE,  
               &points, &n, &resp)) && (resp == UF_UI_OK))  
        {  
            point_data = (UF_CURVE_pt_slope_crvatr_t *)UF_allocate_memory(n *  
                sizeof(UF_CURVE_pt_slope_crvatr_t), &err);  
            spts = (double *)UF_allocate_memory(3 * n * sizeof(double), &err);  
            for (ii = 0; ii < n; ii++)  
            {  
                UF_VEC3_copy(points[ii].pt, point_data[ii].point);  
                spts[ii*3] = point_data[ii].point[0];  
                spts[(ii*3)+1] = point_data[ii].point[1];  
                spts[(ii*3)+2] = point_data[ii].point[2];  
                if ((ii == 0) || (ii == (n-1)))  
                    point_data[ii].slope_type = UF_CURVE_SLOPE_AUTO;  
                else  
                    point_data[ii].slope_type = UF_CURVE_SLOPE_NONE;  
                point_data[ii].crvatr_type = UF_CURVE_CRVATR_NONE;  
            }  
            UF_CALL(UF_CURVE_create_spline_thru_pts(3, flag, n, point_data,  
                    NULL, FALSE, &spline));  
            UF_free(point_data);  
            UF_free(points);  
            UF_free(spts);  
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

> 这段代码是NX的二次开发代码，主要功能是通过用户选择的点创建样条曲线。
>
> 主要步骤包括：
>
> 1. 包含NX API的头文件，定义错误报告函数report_error。
> 2. 定义一个函数ask_true_or_false，用于询问用户是否创建封闭曲线。
> 3. 定义主函数do_it，其中包含以下步骤：询问用户是否创建封闭曲线，并获取选择的点集。为每个点分配内存，并设置点的坐标和斜率类型。调用UF_CURVE_create_spline_thru_pts函数通过这些点创建样条曲线。释放内存。
> 4. 询问用户是否创建封闭曲线，并获取选择的点集。
> 5. 为每个点分配内存，并设置点的坐标和斜率类型。
> 6. 调用UF_CURVE_create_spline_thru_pts函数通过这些点创建样条曲线。
> 7. 释放内存。
> 8. 定义ufusr函数，在NX启动时调用do_it函数，在NX关闭时调用UF_terminate函数。
> 9. 定义ufusr_ask_unload函数，用于在卸载时立即释放内存。
>
> 总体来说，这段代码实现了通过用户选择的点创建样条曲线的功能，具有一定的NX二次开发参考价值。
>
