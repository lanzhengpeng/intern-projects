### 【0003】add parents to list 向列表中添加父组件

#### 代码

```cpp
    static void add_parents_to_list(tag_t component, uf_list_p_t parent_list)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *parents;  
        n = UF_ASSEM_where_is_part_used(component, &parents);  
        for (ii = 0; ii < n; ii++)  
        {  
            UF_CALL(UF_MODL_put_list_item(parent_list, parents[ii]));  
            add_parents_to_list(parents[ii], parent_list);  
        }  
        if (n) UF_free(parents);  
    }

```

#### 代码解析

> 这段代码是NX二次开发中用于获取组件在装配体中的父组件列表的函数。具体功能包括：
>
> 1. 获取当前组件的父组件列表。
> 2. 将父组件添加到提供的父组件列表中。
> 3. 递归调用自身，以获取父组件的父组件，并将它们也添加到列表中。
> 4. 释放内存，防止内存泄露。
>
> 主要特点包括：
>
> 总体来说，这段代码实现了NX装配体中组件的父组件列表的获取功能，通过递归调用实现了组件的所有父组件的获取，并将它们存储在提供的列表中。
>
