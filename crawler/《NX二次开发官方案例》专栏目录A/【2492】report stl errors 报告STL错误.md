### 【2492】report stl errors 报告STL错误

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
    static void report_stl_errors(int n_errors, UF_STD_stl_error_t *error_info)  
    {  
        int  
            ii;  
        char  
            error_text[133];  
        WRITE_D(n_errors);  
        for (ii = 0; ii < n_errors; ii++)  
        {  
            WRITE_D(ii);  
            WRITE_D(error_info[ii].error_code);  
            UF_get_fail_message(error_info[ii].error_code, error_text);  
            WRITE_S(error_text);  
            WRITE_D(error_info[ii].related);  
            WRITE3F(error_info[ii].point1);  
            WRITE3F(error_info[ii].point2);  
        }  
        if (n_errors > 0) UF_free(error_info);  
    }

```

#### 代码解析

> 这段NX Open C++代码主要用于在NX的Listing窗口中输出错误报告。代码的主要功能如下：
>
> 1. 定义了三个宏，分别用于输出整数、字符串和浮点数组到Listing窗口。
> 2. 实现了write_integer_to_listing_window、write_string_to_listing_window和write_double_array_to_listing_window三个函数，用于在Listing窗口中输出不同类型的数据。
> 3. report_stl_errors函数用于报告STL文件导入NX时出现的错误，包括错误数量、错误代码、错误文本、相关对象编号以及错误位置等信息。
> 4. 使用UF_get_fail_message函数获取错误代码对应的错误文本。
> 5. 使用UF_free释放了动态分配的错误信息数组。
> 6. 在输出数据时，通过UF_UI_open_listing_window和UF_UI_write_listing_window两个函数与NX的用户界面交互。
>
> 总体来说，这段代码为NX的STL文件导入提供了一个简单的错误报告机制，便于用户查看和调试STL文件中的问题。
>
