### 【0271】ask next feature 获取下一个特征

#### 代码

```cpp
    static tag_t ask_next_feature(tag_t part, tag_t feat)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE) return feat;  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX二次开发的函数，其功能是遍历指定部件中的特征，并返回下一个激活状态的特征。
>
> 主要步骤包括：
>
> 1. 调用UF_OBJ_cycle_objs_in_part()函数，循环遍历部件中的特征，并返回下一个特征。
> 2. 判断返回的特征是否为NULL_TAG，如果不是，则调用UF_OBJ_ask_status()函数，检查特征的状态是否为激活状态。
> 3. 如果特征为激活状态，则返回该特征；否则继续循环，直到找到激活状态的特征为止。
> 4. 如果所有特征都遍历完毕，没有找到激活状态的特征，则最终返回NULL_TAG。
>
> 总体来说，这个函数实现了遍历部件中的特征，并返回下一个激活状态的特征的功能。
>
