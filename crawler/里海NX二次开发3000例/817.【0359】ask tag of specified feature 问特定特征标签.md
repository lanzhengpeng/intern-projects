### 【0359】ask tag of specified feature 问特定特征标签

#### 代码

```cpp
    static tag_t ask_next_feature(tag_t part, tag_t feat)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
            if (UF_OBJ_ask_status(feat) == 3) return feat;  
        return NULL_TAG;  
    }  
    static tag_t ask_tag_of_specified_feature(char *name)  
    {  
        tag_t  
            feat = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            *this_name;  
        while ((feat = ask_next_feature(part, feat)) != NULL_TAG)  
        {  
            if (!UF_CALL(UF_MODL_ask_feat_name(feat, &this_name)))  
            {  
                if (!strcmp(name, this_name))  
                    return(feat);  
            }  
        }  
    }

```

#### 代码解析

> 这段代码是NX二次开发的示例代码，主要用于在NX模型中查询特定名称的特征。
>
> 主要逻辑包括：
>
> 1. 定义一个静态函数ask_next_feature，用于遍历零件中的所有特征，并返回下一个激活的特征。
> 2. 定义一个静态函数ask_tag_of_specified_feature，用于查询特定名称的特征。它首先获取当前显示的零件，然后循环调用ask_next_feature获取下一个特征，并检查名称是否与指定的特征名称相同。如果找到，则返回该特征的标签。
> 3. 使用UF_OBJ_cycle_objs_in_part遍历零件中的所有对象，UF_OBJ_ask_status检查对象的状态，UF_MODL_ask_feat_name获取特征名称，strcmp比较字符串。
> 4. 如果找到名称匹配的特征，则返回该特征的标签；如果没有找到，最终返回NULL_TAG。
> 5. 通过两个静态函数的组合，实现了按名称查询特定特征的功能。
>
> 总体来说，这段代码利用NX提供的API，实现了在零件中按名称查询激活特征的功能。代码结构清晰，逻辑简单，是一个典型的NX二次开发示例。
>
