### 【2064】report dimension line information 报告尺寸线信息

#### 代码

```cpp
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
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
            blanks[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            UF_CALL(UF_UI_write_listing_window(msg));  
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
            UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    static void report_dimension_line_information(tag_t dim)  
    {  
        int  
            ii,  
            jj,  
            subtype;  
        double  
            origin[3];  
        UF_DRF_dim_info_t  
            *info;  
        UF_CALL(UF_DRF_ask_dim_info(dim, &subtype, origin, &info));  
        for (ii = 0; ii<info->num_text; ii++)  
            for (jj = 0; jj < info->text_info[ii].num_lines; jj++)  
                WRITE_S(info->text_info[ii].text[jj].string);  
        WRITE_D(info->num_lines);  
        for (ii = 0; ii < info->num_lines; ii++)  
        {  
            WRITE_D(info->dim_line_info->line_pnts[ii].num_segments);  
            for (jj=0; jj<info->dim_line_info->line_pnts[ii].num_segments; jj++)  
            {  
                WRITE3F(&info->dim_line_info->line_pnts[ii].segment_pnts[jj*6]);  
                WRITE3F(&info->dim_line_info->line_pnts[ii].segment_pnts[jj*6+3]);  
            }  
        }  
        WRITE_D(info->dim_line_info->num_assoc_objs);  
        for (ii = 0; ii < info->dim_line_info->num_assoc_objs; ii++)  
        {  
            WRITE_D(info->dim_line_info->assoc_objs[ii].assoc_object_tag);  
            WRITE_D(info->dim_line_info->assoc_objs[ii].assoc_object_view_tag);  
            WRITE_D(info->dim_line_info->assoc_objs[ii].assoc_type);  
            WRITE_D(info->dim_line_info->assoc_objs[ii].assoc_modifier);  
        }  
        printf("\n");  
        UF_CALL(UF_DRF_free_dimension(&info));  
    }

```

#### 代码解析

> 这是段用于获取和打印NX中尺寸线信息的二次开发代码。
>
> 主要功能包括：
>
> 1. 定义了几个宏，用于打开列表窗口并写入整型、字符串和浮点数数组。
> 2. report_dimension_line_information函数用于获取指定尺寸线的详细信息，包括文字、线段数、线段端点坐标等。
> 3. 使用NX提供的UF_DRF_ask_dim_info函数来获取尺寸线的详细信息，然后遍历并打印出来。
> 4. 获取每个文字块的所有文字行，并打印出来。
> 5. 打印尺寸线的线段数，然后遍历每个线段，打印出线段的起始和结束坐标。
> 6. 打印与尺寸线关联的对象数量，然后遍历每个关联对象，打印出对象标签、视图标签、关联类型和关联修饰符。
> 7. 在打印完所有信息后，释放获取的尺寸线信息内存。
> 8. 整个过程在列表窗口中进行，方便查看。
>
> 通过调用report_dimension_line_information函数并传入指定的尺寸线tag，可以获取该尺寸线的详细信息，并打印在列表窗口中，方便进行二次开发调试。
>
