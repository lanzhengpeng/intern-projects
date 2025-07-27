### 【0219】ask feat first object 查询特征第一个对象

#### 代码

```cpp
    static tag_t ask_feat_first_object(tag_t feat)  
    {  
        int  
            n;  
        tag_t  
            first = NULL_TAG,  
            *objects;  
        UF_CALL(UF_MODL_ask_feat_object(feat, &n, &objects));  
        if (n > 0)  
        {  
            first = objects[0];  
            UF_free(objects);  
        }  
        return first;  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发的功能，主要实现了以下功能：
>
> 1. 获取NX特征的对象列表。
> 2. 获取特征对象列表中的第一个对象。
> 3. 释放内存。
>
> 具体步骤如下：
>
> 1. 调用UF_MODL_ask_feat_object函数，获取特征feat的对象列表，以及对象数量n和对象数组objects。
> 2. 判断对象数量n是否大于0，如果是，则获取数组中的第一个对象first。
> 3. 释放对象数组objects占用的内存。
> 4. 返回获取的第一个对象first。
>
> 该函数的主要作用是获取特征feat的第一个关联对象，并释放内存。
>
