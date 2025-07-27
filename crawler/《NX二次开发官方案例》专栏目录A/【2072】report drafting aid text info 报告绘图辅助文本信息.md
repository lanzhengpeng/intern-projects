### 【2072】report drafting aid text info 报告绘图辅助文本信息

#### 代码

```cpp
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    static void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
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
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
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
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static void report_drafting_aid_text_info(tag_t draft_aid)  
    {  
        int  
            ii,  
            jj,  
            n_strings;  
        UF_DRF_draft_aid_text_info_t  
            *text_info = NULL;  
        UF_CALL(UF_DRF_ask_draft_aid_text_info(draft_aid, &n_strings, &text_info));  
        WRITE_D(n_strings);  
        for (ii = 0; ii < n_strings; ii++)  
        {  
            WRITE_D(ii);  
            WRITE_D(text_info[ii].text_type);  
            WRITE_D(text_info[ii].text_font);  
            WRITE_F(text_info[ii].size);  
            WRITE_F(text_info[ii].angle);  
            WRITE3F(text_info[ii].origin);  
            display_temporary_point(text_info[ii].origin);  
            WRITE_F(text_info[ii].length);  
            WRITE_F(text_info[ii].height);  
            WRITE_F(text_info[ii].distance);  
            WRITE_F(text_info[ii].aspect_ratio);  
            WRITE_F(text_info[ii].gap);  
            WRITE_F(text_info[ii].line_spacing);  
            WRITE_D(text_info[ii].num_lines);  
            for (jj=0; jj<text_info[ii].num_lines; jj++)  
            {  
                WRITE_D(jj);  
                WRITE_D(text_info[ii].text[jj].num_chars);  
                WRITE_S(text_info[ii].text[jj].string);  
                WRITE_D(text_info[ii].text[jj].num_ints);  
                WRITE_D(text_info[ii].text[jj].full_num_chars);  
                WRITE_S(text_info[ii].text[jj].full_string);  
            }  
        }  
        if( text_info != NULL )  
            UF_CALL(UF_DRF_free_text(n_strings, &text_info));  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 定义了一系列宏，用于将变量值打印到列表窗口中，包括字符串、浮点数、整数和浮点数数组。
> 2. 实现了display_temporary_point函数，用于在指定坐标上显示一个临时的点。
> 3. 实现了report_drafting_aid_text_info函数，用于报告绘图辅助文本信息。该函数首先获取文本信息，然后遍历每个文本，打印出其类型、字体、大小、角度、原点、长度、高度、距离、宽高比、间隙、行间距、行数等属性。对于每行文本，还会打印出字符数、字符串内容、整数数、完整字符数和完整字符串内容。在最后，释放了文本信息内存。
> 4. 代码通过宏和函数封装，提高了代码的复用性和可读性。通过打印文本信息，可以帮助调试和了解绘图辅助文本的设置情况。
>
