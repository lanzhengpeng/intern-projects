### 【2499】report tabular note cell preferences 报告表格注释单元格首选项

#### 代码

```cpp
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
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
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITEnD(n,X) (write_integer_array_to_listing_window(#X, X, 1, n))  
    static void write_integer_array_to_listing_window(char *name, int *array,  
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
                sprintf(msg, "%d", array[kk++]);  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void report_tabular_note_cell_preferences(  
        UF_TABNOT_cell_prefs_p_t cell_prefs)  
    {  
        WRITE_D(cell_prefs->format);  
        WRITE_D(cell_prefs->precision);  
        WRITE_L(cell_prefs->is_a_formula);  
        WRITE_D(cell_prefs->zero_display);  
        WRITE_D(cell_prefs->text_font);  
        WRITE_F(cell_prefs->text_height);  
        WRITE_F(cell_prefs->text_aspect_ratio);  
        WRITE_D(cell_prefs->horiz_just);  
        WRITE_D(cell_prefs->vert_just);  
        WRITE_F(cell_prefs->text_angle);  
        WRITE_F(cell_prefs->text_slant);  
        WRITE_L(cell_prefs->is_vertical);  
        WRITE_L(cell_prefs->is_italic);  
        WRITE_L(cell_prefs->strikethru);  
        WRITE_F(cell_prefs->line_space_factor);  
        WRITE_F(cell_prefs->char_space_factor);  
        WRITE_L(cell_prefs->is_hidden);  
        WRITE_D(cell_prefs->text_color);  
        WRITE_D(cell_prefs->text_density);  
        WRITEnD(3, cell_prefs->bottom_line_cfw);  
        WRITEnD(3, cell_prefs->right_line_cfw);  
        WRITE_D(cell_prefs->nm_fit_methods);  
        WRITEnD(cell_prefs->nm_fit_methods, cell_prefs->fit_methods);  
        WRITE_D(cell_prefs->referenced_spreadsheet);  
        WRITE_D(cell_prefs->ss_sheet);  
        WRITE_D(cell_prefs->ss_row);  
        WRITE_D(cell_prefs->ss_col);  
        WRITE_S(cell_prefs->prefix);  
        WRITE_S(cell_prefs->suffix);  
        WRITE_S(cell_prefs->formula_suffix);  
        WRITE_S(cell_prefs->url);  
    }

```

#### 代码解析

> 这段代码定义了一系列宏，用于向NX的Listing窗口输出不同类型的数据。
>
> 主要内容包括：
>
> 1. 定义了5个宏：WRITE_D、WRITE_L、WRITE_F、WRITEnD、WRITE_S，分别用于输出整型、逻辑型、浮点型、整型数组和字符串。
> 2. 为每个宏定义了对应的函数，这些函数负责打开Listing窗口，格式化要输出的数据，并写入窗口。
> 3. 定义了一个report_tabular_note_cell_preferences函数，用于输出表格注释单元的格式偏好设置。它使用上述宏依次输出各个字段的值。
> 4. 这段代码通过宏封装简化了输出过程，提高了代码的复用性，并且使得输出格式一致。
> 5. 这段代码可用于调试，通过输出关键数据来观察程序的运行状态。
>
