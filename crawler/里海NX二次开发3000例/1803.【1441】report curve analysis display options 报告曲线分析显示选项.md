### 【1441】report curve analysis display options 报告曲线分析显示选项

#### 代码

```cpp
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    static void report_curve_analysis_display_options(  
        UF_CURVE_analysis_display_p_t analysis_display_options)  
    {  
        WRITE_F(analysis_display_options->curvature_comb_scale_factor);  
        WRITE_L(analysis_display_options->show_control_polygon);  
        WRITE_L(analysis_display_options->show_curvature_comb);  
        WRITE_L(analysis_display_options->show_inflections);  
        WRITE_L(analysis_display_options->show_peaks);  
        WRITE_F(analysis_display_options->comb_density);  
        WRITE_F(analysis_display_options->ustart);  
        WRITE_F(analysis_display_options->uend);  
        WRITE_L(analysis_display_options->use_max_length);  
        WRITE_F(analysis_display_options->max_length);  
        WRITE_D(analysis_display_options->proj_choice);  
        WRITE3F(analysis_display_options->specified_dir);  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能是用于报告曲线分析显示选项。其中，定义了一系列宏，用于将不同类型的数据输出到列表窗口中，包括浮点数、逻辑值、整数和浮点数组。此外，还定义了一个函数report_curve_analysis_display_options，用于报告曲线分析显示选项，包括曲率组合缩放因子、是否显示控制多边形、曲率梳、拐点、峰值、梳密度、起始和结束参数、是否使用最大长度、最大长度、投影选择以及指定的方向。通过这些宏和函数，可以方便地将曲线分析显示选项输出到列表窗口，以方便用户查看和调整。
>
