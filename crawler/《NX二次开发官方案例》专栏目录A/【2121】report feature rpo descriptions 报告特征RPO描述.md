### 【2121】report feature rpo descriptions 报告特征RPO描述

#### 代码

```cpp
    static void report_feature_name(tag_t feat)  
    {  
        char  
            *string;  
        UF_CALL(UF_MODL_ask_feat_name(feat, &string));  
        printf("%d = %s\n", feat, string);  
        UF_free(string);  
    }  
    static void report_feature_rpo_descriptions(tag_t feat)  
    {  
        int  
            ii,  
            n_exps;  
        tag_t  
            *exps;  
        char  
            **descriptions;  
        report_feature_name(feat);  
        UF_CALL(UF_MODL_ask_rpo_desc_of_feat(feat, &n_exps, &descriptions, &exps));  
        for (ii = 0; ii < n_exps; ii++)  
            printf("  %d.  %d = %s\n", ii+1, exps[ii], descriptions[ii]);  
        UF_free(exps);  
        UF_free_string_array(n_exps, descriptions);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，用于查询NX特征的相关信息。
>
> 1. report_feature_name函数：用于查询并打印特征的名字。它通过UF_MODL_ask_feat_name函数获取特征名，并使用printf打印特征号和特征名。
> 2. report_feature_rpo_descriptions函数：用于查询并打印特征的参考点顺序描述。它首先调用report_feature_name打印特征名，然后通过UF_MODL_ask_rpo_desc_of_feat函数获取特征的所有参考点顺序描述，并使用printf按顺序打印描述信息。
> 3. UF_free用于释放内存，避免内存泄露。
> 4. UF_CALL宏用于简化UF函数的调用，自动处理返回码。
>
> 这段代码实现了查询NX特征名和参考点顺序描述的功能，并以结构化的方式打印出来。
>
