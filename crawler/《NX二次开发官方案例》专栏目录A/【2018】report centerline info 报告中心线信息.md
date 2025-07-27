### 【2018】report centerline info 报告中心线信息

#### 代码

```cpp
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
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
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
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
    static void report_centerline_info(tag_t cline)  
    {  
        int  
            ii,  
            jj;  
        double  
            centerline_origin[3];  
        UF_DRF_valid_cline_form_t  
            centerline_type;  
        UF_DRF_centerline_info_t  
            *centerline_info;  
        if (UF_CALL(UF_DRF_ask_centerline_info(cline, &centerline_type,  
            centerline_origin, &centerline_info))) return;  
        WRITE_D(centerline_type);  
        WRITE3F(centerline_origin);  
        WRITE_D(centerline_info->num_lines);  
        for (ii = 0; ii < centerline_info->num_lines; ii++)  
        {  
            WRITE_D(ii);  
            WRITE_D(centerline_info->line_info[ii].sequence_number);  
            WRITE_D(centerline_info->line_info[ii].num_symbols);  
            for (jj = 0; jj < centerline_info->line_info[ii].num_symbols;  
                jj++)  
            {  
                WRITE_D(jj);  
                WRITE_D(centerline_info->line_info[ii].symbol_data[jj].  
                    master);  
                WRITE_L(centerline_info->line_info[ii].symbol_data[jj].  
                    is_gap);  
                WRITE3F(centerline_info->line_info[ii].symbol_data[jj].  
                    origin);  
                WRITE_F(centerline_info->line_info[ii].symbol_data[jj].  
                    parameter_on_element);  
                WRITE_F(centerline_info->line_info[ii].symbol_data[jj].  
                    symbol_length);  
                WRITE_F(centerline_info->line_info[ii].symbol_data[jj].  
                    symbol_height);  
                WRITE_D(centerline_info->line_info[ii].symbol_data[jj].  
                    creation_mask);  
                WRITE_D(centerline_info->line_info[ii].symbol_data[jj].  
                    record_number);  
                WRITE_D(centerline_info->line_info[ii].symbol_data[jj].  
                    segment_number);  
                WRITE_L(centerline_info->line_info[ii].symbol_data[jj].  
                    is_mirrored);  
                WRITE_L(centerline_info->line_info[ii].symbol_data[jj].  
                    is_reflected);  
            }  
            WRITE_D(centerline_info->line_info[ii].num_segments);  
            for (jj = 0;  
                 jj < centerline_info->line_info[ii].num_segments * 6;  
                 jj = jj + 3)  
            {  
                WRITE_D(jj);  
                WRITE3F(&centerline_info->line_info[ii].segment_pnts[jj]);  
            }  
        }  
        WRITE_D(centerline_info->num_arcs);  
        WRITE_D(centerline_info->num_assoc_objs);  
        /*  This function is working but unfinished - aiw 译:这个函数正在运行但尚未完成。 */  
        UF_CALL(UF_DRF_free_centerline(&centerline_info));  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发，用于报告中心线的详细信息。
>
> 主要功能包括：
>
> 1. 使用UF_DRF_ask_centerline_info函数获取中心线的类型、原点等信息，以及中心线包含的线条、符号等详细信息。
> 2. 使用自定义的WRITE系列宏，将获取的中心线信息格式化输出到日志窗口中。这些宏用于输出不同类型的数据，例如整数、浮点数、布尔值等。
> 3. report_centerline_info函数遍历中心线的所有线条和符号，使用WRITE系列宏将它们的详细信息输出到日志窗口中。
> 4. 注意，代码最后提示这个函数正在运行但尚未完成，说明作者可能还会继续完善这个函数。
>
> 总体来说，这段代码实现了获取和格式化输出中心线详细信息的功能，为NX的二次开发提供了方便。
>
