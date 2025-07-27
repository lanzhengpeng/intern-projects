### 【3061】write exp array to listing window 将表达式数组写入列表窗口

#### 代码

```cpp
    #define WRITEnE(n,X) (write_exp_array_to_listing_window(#X, X, n))  
    void write_exp_array_to_listing_window(char *title, tag_t *exps, int n_exps)  
    {  
        int  
            ii;  
        char  
            *string;  
        for (ii = 0; ii < n_exps; ii++)  
        {  
            UF_CALL(UF_MODL_ask_exp_tag_string(exps[ii], &string));  
            ECHO("%s[%d] = %d - %s\n", title, ii, exps[ii], string);  
            UF_free(string);  
        }  
    }

```

#### 代码解析

> 根据代码，这是NX Open C++ API编写的二次开发代码，主要用于将NX表达式数组的内容输出到列表窗口。
>
> 主要功能包括：
>
> 1. 使用宏定义WRITEnE(n,X)，用于调用write_exp_array_to_listing_window函数，以输出数组X的内容。
> 2. write_exp_array_to_listing_window函数接收标题、表达式数组及数组大小，遍历数组，并调用UF_MODL_ask_exp_tag_string函数获取每个表达式的字符串表示，然后输出标题、下标、表达式ID及字符串表示到列表窗口。
> 3. 每次调用UF_MODL_ask_exp_tag_string后，都会释放返回的字符串内存，以避免内存泄漏。
> 4. 输出格式为"标题[下标] = 表达式ID - 表达式字符串"，例如"exprs[0] = 1 - radius"。
> 5. 该代码主要用于调试，能够直观地查看表达式数组的内容，便于发现问题。
> 6. 使用了NX Open C++ API中的模型访问模块函数，需要包含相应的头文件。
>
> 总体而言，这段代码提供了输出表达式数组内容到列表窗口的便捷方法，适合用于调试和查看NX表达式信息。
>
