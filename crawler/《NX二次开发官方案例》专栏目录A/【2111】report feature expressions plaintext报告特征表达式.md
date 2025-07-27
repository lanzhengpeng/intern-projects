### 【2111】report feature expressions plaintext报告特征表达式

#### 代码

```cpp
    static void report_feature_name_and_type(char *title, tag_t feat)  
    {  
        char  
            msg[MAX_LINE_SIZE+1],  
            *name,  
            *type;  
        if (!UF_CALL(UF_MODL_ask_feat_name(feat, &name)) &&  
            !UF_CALL(UF_MODL_ask_feat_type(feat, &type)))  
        {  
            sprintf(msg, "%s = %d %s (%s)\n", title, feat, name, type);  
            ECHO(msg);  
            UF_free(name);  
            UF_free(type);  
        }  
    }  
    #define RFE(X) report_feature_expressions(#X, X)  
    static void report_feature_expressions(char *which, tag_t feat)  
    {  
        int  
            ii,  
            n_exps;  
        tag_t  
            *exps,  
            master;  
        char  
            **descs,  
            msg[MAX_LINE_SIZE+1],  
            *string,  
            *type;  
        report_feature_name_and_type(which, feat);  
        if(!UF_CALL(UF_MODL_ask_exp_desc_of_feat(feat, &n_exps, &descs, &exps)))  
        {  
            for (ii = 0; ii < n_exps; ii++)  
            {  
                if (!UF_CALL(UF_MODL_ask_exp_tag_string(exps[ii], &string)))  
                {  
                    sprintf(msg, "  %d.  %s (%s)\n", ii+1, string, descs[ii]);  
                    UF_free(string);  
                    ECHO(msg);  
                }  
            }  
            UF_free(exps);  
            UF_free_string_array(n_exps, descs);  
        }  
        UF_CALL(UF_MODL_ask_feat_type(feat, &type));  
        if (!strcmp(type, "INSTANCE"))  
        {  
            UF_CALL(UF_MODL_ask_master(feat, &master));  
            RFE(master);  
        }  
        UF_free(type);  
    }

```

#### 代码解析

> 这段NX Open C++代码用于报告NX特征的相关信息，包括特征名称、类型、表达式描述和实例化特征的主特征。代码主要包含以下几个部分：
>
> 1. report_feature_name_and_type函数：用于报告特征名称和类型。
> 2. report_feature_expressions函数：用于报告特征的表达式描述，并递归调用自身来报告实例化特征的主特征。
> 3. RFE宏：用于简化对report_feature_expressions函数的调用。
> 4. 通过调用UF_MODL_ask_feat_name、UF_MODL_ask_feat_type、UF_MODL_ask_exp_desc_of_feat等NX API获取特征信息。
> 5. 使用sprintf和ECHO宏输出格式化的特征信息。
> 6. 释放使用NX API获取的内存。
> 7. 根据特征类型，递归调用report_feature_expressions函数报告实例化特征的主特征。
> 8. 使用UF_free释放NX API分配的内存。
>
> 总的来说，这段代码实现了获取和报告NX特征详细信息的功能，其中通过递归调用自身来报告实例化特征的主特征，实现了对特征层次结构的遍历。
>
