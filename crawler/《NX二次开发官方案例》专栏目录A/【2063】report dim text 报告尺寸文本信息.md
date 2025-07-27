### 【2063】report dim text 报告尺寸文本信息

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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void report_dim_text(tag_t dim)  
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
        {  
            WRITE_D(ii);  
            for (jj = 0; jj < info->text_info[ii].num_lines; jj++)  
            {  
                WRITE_D(ii);  
                WRITE_S(info->text_info[ii].text[jj].string);  
            }  
        }  
        UF_CALL(UF_DRF_free_dimension(&info));  
    }

```

#### 代码解析

> 这段代码是用于NX（Siemens NX）的二次开发，其功能是报告一个维度对象的文本信息。具体包括：
>
> 1. 定义了两个宏WRITE_D和WRITE_S，分别用于将整数和字符串写入列表窗口。
> 2. 实现了write_integer_to_listing_window和write_string_to_listing_window函数，用于向列表窗口写入整数和字符串信息。
> 3. 定义了report_dim_text函数，用于报告一个维度对象的文本信息。它首先调用UF_DRF_ask_dim_info函数获取维度对象的文本信息，然后遍历每个文本行，将其写入列表窗口。
> 4. 在report_dim_text函数中，通过WRITE_D宏将文本行索引写入列表窗口，通过WRITE_S宏将文本行内容写入列表窗口。
> 5. 最后，调用UF_DRF_free_dimension释放获取的维度对象信息。
>
> 总体来说，这段代码通过宏和函数封装，实现了将NX维度对象的文本信息以易于阅读的方式报告给用户，提高了NX二次开发的易用性。
>
