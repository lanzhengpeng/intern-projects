### 【3081】ask next feature of type prefix 根据给定类型前缀，询问下一个特征

#### 代码

```cpp
    static tag_t ask_next_feature_of_type_prefix(tag_t part, char *type_prefix,  
        tag_t feat)  
    {  
        char  
            *this_type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
        {  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE)  
            {  
                UF_CALL(UF_MODL_ask_feat_type(feat, &this_type));  
                if (strstr(this_type, type_prefix) == this_type)  
                {  
                    UF_free(this_type);  
                    return feat;  
                }  
                UF_free(this_type);  
            }  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段NX二次开发代码用于遍历指定部件中的特征，并找到类型名称以指定前缀开头的第一个特征。
>
> 主要步骤包括：
>
> 1. 定义函数ask_next_feature_of_type_prefix，用于获取部件中类型名称以指定前缀开头的下一个特征。
> 2. 使用UF_OBJ_cycle_objs_in_part函数遍历指定部件中的所有特征，直到找到类型名称以指定前缀开头的特征。
> 3. 在循环中，使用UF_OBJ_ask_status检查特征的状态是否为UF_OBJ_ALIVE。
> 4. 如果特征状态为UF_OBJ_ALIVE，则使用UF_MODL_ask_feat_type获取特征类型字符串。
> 5. 检查特征类型字符串是否以指定的type_prefix字符串开头。
> 6. 如果找到符合条件的特征，则释放类型字符串内存，并返回该特征。
> 7. 如果遍历结束没有找到符合条件的特征，则返回NULL_TAG。
>
> 这段代码的作用是在NX部件中找到类型名称以指定前缀开头的特征，可以用于NX二次开发中需要操作特定类型特征的场景。
>
