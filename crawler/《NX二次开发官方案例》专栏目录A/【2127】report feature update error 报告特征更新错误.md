### 【2127】report feature update error 报告特征更新错误

#### 代码

```cpp
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static void report_feature_update_error(tag_t feature)  
    {  
        char  
            *err,  
            *name;  
        if (!UF_CALL(UF_MODL_ask_update_error_message(feature, &err))  
            && (err != NULL)  
            && !UF_CALL(UF_MODL_ask_feat_name(feature, &name)))  
        {  
            WRITE(name);  
            WRITE(" - ");  
            WRITE(err);  
            WRITE("\n");  
            UF_free(name);  
            UF_free(err);  
        }  
    }

```

#### 代码解析

> 这是段NX Open C++二次开发代码，用于在NX中处理特征更新错误并输出错误信息。
>
> 代码主要包含以下部分：
>
> 1. 定义宏WRITE(X)，用于打开NX的列表窗口并写入信息X。
> 2. 定义静态函数report_feature_update_error(tag_t feature)，用于报告特征更新错误。
> 3. 在函数中，首先获取特征更新错误的错误信息字符串err和特征名称字符串name。
> 4. 如果获取成功，则调用WRITE宏将特征名称、错误分隔符、错误信息写入列表窗口，并换行。
> 5. 最后，释放获取的字符串内存。
>
> 该代码通过调用NX Open C++的UF_MODL接口函数，获取特征更新错误信息，并以用户友好的方式输出，从而帮助开发者调试特征更新相关的代码。
>
