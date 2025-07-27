### 【2116】report feature name type and sysname 报告特征名称、类型和系统名称

#### 代码

```cpp
    static void report_feature_name_type_and_sysname(tag_t feature)  
    {  
        char  
            *feature_name,  
            *feature_sysname,  
            *feature_type;  
        UF_CALL(UF_MODL_ask_feat_name( feature, &feature_name ));  
        WRITE_S(feature_name);  
        UF_free(feature_name);  
        UF_CALL(UF_MODL_ask_feat_sysname( feature, &feature_sysname ));  
        WRITE_S(feature_sysname);  
        UF_free(feature_sysname);  
        UF_CALL(UF_MODL_ask_feat_type( feature, &feature_type ));  
        WRITE_S(feature_type);  
        UF_free(feature_type);  
    }

```

#### 代码解析

> 这段代码是NX二次开发中的一段函数，主要功能是获取并输出特征（feature）的名称、系统名称和类型。
>
> 代码主要包含以下步骤：
>
> 1. 定义了三个字符指针，用于存储特征名称、系统名称和类型。
> 2. 使用UF_MODL_ask_feat_name函数获取特征的名称，并输出。
> 3. 使用UF_MODL_ask_feat_sysname函数获取特征的系统名称，并输出。
> 4. 使用UF_MODL_ask_feat_type函数获取特征的类型，并输出。
> 5. 在输出每个信息后，使用UF_free释放相应的内存。
> 6. 函数最后返回，完成特征的名称、系统名称和类型的获取与输出。
>
> 这段代码通过UF_MODL_ask_feat_name、UF_MODL_ask_feat_sysname和UF_MODL_ask_feat_type三个函数，实现了对特征名称、系统名称和类型的查询，并进行了输出。通过这段代码，可以获取到特征的相关信息，为后续的二次开发提供支持。
>
