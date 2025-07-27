### 【2117】report feature name 报告特征名称

#### 代码

```cpp
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
    }  
    static void report_feature_name(tag_t feat)  
    {  
        char  
            *feature;  
        if (!UF_CALL(UF_MODL_ask_feat_name(feat, &feature)))  
        {  
            WRITE_S(feature);  
            UF_free(feature);  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API的二次开发示例代码，其主要功能是输出特征名称到NX的日志窗口中。
>
> 代码首先定义了一个宏WRITE_S，用于输出字符串到日志窗口。
>
> 然后定义了一个函数write_string_to_listing_window，用于输出标题和字符串到日志窗口。如果字符串为NULL，则只输出标题。
>
> 最后定义了函数report_feature_name，用于获取指定特征的特征名称，并调用WRITE_S输出到日志窗口。在输出后，使用UF_free释放内存。
>
> 总的来说，这段代码的作用是封装了NX API中的函数，用于输出特征名称，并提供了方便的宏来简化代码。
>
