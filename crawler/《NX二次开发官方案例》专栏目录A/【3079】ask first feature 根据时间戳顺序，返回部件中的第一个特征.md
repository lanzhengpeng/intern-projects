### 【3079】ask first feature 根据时间戳顺序，返回部件中的第一个特征

#### 代码

```cpp
    /*  This function will return the first feature in the part based on Timestamp order 译:这个函数将根据时间戳顺序返回部件中的第一个特征。 */  
    static tag_t ask_first_feature(tag_t part)  
    {  
        tag_t  
            feat = NULL_TAG;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE) return feat;  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX二次开发中用于查询部件中第一个特征的函数。其主要功能如下：
>
> 1. 定义了一个名为ask_first_feature的静态函数，用于返回部件中的第一个特征。
> 2. 使用循环遍历部件中的特征对象，并按时间戳顺序进行遍历。
> 3. 在循环中，使用UF_OBJ_cycle_objs_in_part函数遍历部件中的特征对象，并获取特征对象。
> 4. 检查遍历到的特征对象是否处于活动状态，如果处于活动状态，则返回该特征对象。
> 5. 如果遍历完所有特征对象仍未找到活动状态的特征，则返回NULL。
> 6. 该函数通过按时间戳顺序遍历部件中的特征，以找到第一个活动状态的特征，并返回该特征对象。
> 7. 这段代码展示了NX二次开发中查询特征对象的基本思路，即使用循环遍历并按条件过滤，以获取满足条件的特征对象。
> 8. 该函数的返回值是一个特征对象，如果部件中没有活动状态的特征，则返回NULL。
> 9. 通过调用该函数，可以方便地获取NX部件中的第一个特征对象，以进行后续处理。
>
