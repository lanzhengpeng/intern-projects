### 【2009】report body features 报告体特征

#### 代码

```cpp
    static report_body_features(tag_t body)  
    {  
        int  
            count,  
            ii,  
            jj;  
        UF_MODL_features_p_t  
            *features_node;  
        if (!UF_CALL(UF_MODL_ask_body_features(body, &count, &features_node))  
            && (count > 0))  
        {  
            for (ii = 0; ii < count; ii++)  
            {  
                WRITE_D(ii);  
                WRITE_D(features_node[ii]->feat_count);  
                for (jj = 0; jj < features_node[ii]->feat_count; jj++)  
                    WRITE_D(features_node[ii]->feat_tags[jj]);  
                WRITE_S(features_node[ii]->feat_type);  
                UF_free(features_node[ii]->feat_tags);  
                UF_free(features_node[ii]);  
            }  
            UF_free(features_node);  
        }  
    }

```

#### 代码解析

> 这段代码的主要功能是遍历NX CAD模型中的body实体，获取其特征信息并输出。
>
> 主要步骤包括：
>
> 1. 调用UF_MODL_ask_body_features函数，传入body实体的tag，获取特征信息，包括特征数量和特征指针数组。
> 2. 遍历特征指针数组，输出每个特征的详细信息，包括特征索引、特征数量、特征标签数组、特征类型。
> 3. 释放特征标签数组和特征指针数组的内存。
> 4. 释放特征指针数组的内存。
>
> 通过这个函数，可以获取CAD模型中每个body实体的特征信息，对于后续的模型处理和分析非常有用。
>
