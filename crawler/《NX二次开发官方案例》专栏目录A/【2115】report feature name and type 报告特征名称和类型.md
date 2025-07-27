### 【2115】report feature name and type 报告特征名称和类型

#### 代码

```cpp
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_FNT(X) report_feature_name_and_type(#X, X)  
    static void report_feature_name_and_type(char *which, tag_t feature)  
    {  
        char  
            *name,  
            *type,  
            msg[MAX_LINE_LENGTH+1];  
        if (!UF_CALL(UF_MODL_ask_feat_name(feature, &name)) &&  
            !UF_CALL(UF_MODL_ask_feat_type(feature, &type)))  
        {  
            sprintf(msg, "%s - tag_t %d - %s (%s)\n", which, feature, name, type);  
            WRITE(msg);  
            UF_free(name);  
            UF_free(type);  
        }  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能是报告特征名称和类型，并写入列表窗口。
>
> 1. 首先，定义了两个宏：
>
> 1. report_feature_name_and_type函数接受特征名称和特征标签作为参数，用于查询特征的名称和类型，并将信息格式化后输出到列表窗口。
> 2. 使用UF_MODL_ask_feat_name函数查询特征的名称，并使用UF_MODL_ask_feat_type函数查询特征的类型。
> 3. 将特征名称、特征标签、特征类型等拼接成字符串，然后通过WRITE宏写入列表窗口。
> 4. 最后，释放查询到的特征名称和类型字符串的内存。
>
> 这段代码的主要作用是获取NX模型中的特征信息，并以格式化的方式输出到列表窗口，从而方便开发者查看特征信息。
>
