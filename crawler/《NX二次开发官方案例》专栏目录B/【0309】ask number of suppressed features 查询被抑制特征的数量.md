### 【0309】ask number of suppressed features 查询被抑制特征的数量

#### 代码

```cpp
    static int ask_number_of_suppressed_features(void)  
    {  
        int  
            count;  
        uf_list_p_t  
            feature_list;  
        UF_CALL(UF_MODL_ask_suppress_list(&feature_list));  
        UF_CALL(UF_MODL_ask_list_count(feature_list, &count));  
        UF_CALL(UF_MODL_delete_list(&feature_list));  
        return (count);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，用于获取NX中当前被抑制的特征数量。
>
> 代码的主要逻辑包括以下几个步骤：
>
> 1. 调用UF_MODL_ask_suppress_list()函数，获取当前被抑制的特征链表。
> 2. 调用UF_MODL_ask_list_count()函数，统计链表中的特征数量，并将其赋值给count变量。
> 3. 调用UF_MODL_delete_list()函数，删除并释放链表内存。
> 4. 返回count变量，即当前被抑制的特征数量。
>
> 通过这段代码，可以快速获取NX中当前被抑制的特征数量，从而了解模型的简化情况。在NX二次开发中，统计被抑制的特征数量是一个常见需求，这段代码提供了一个简洁的实现方式。
>
