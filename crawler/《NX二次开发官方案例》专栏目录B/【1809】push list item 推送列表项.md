### 【1809】push list item 推送列表项

#### 代码

```cpp
    static uf_list_p_t push_list_item(uf_list_p_t old_list, tag_t new_start)  
    {  
        uf_list_p_t  
            new_list;  
        UF_CALL(UF_MODL_create_list(&new_list));  
        new_list->eid = new_start;  
        new_list->next = old_list;  
        return new_list;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API中用于创建和操作NX列表的一个函数。
>
> 主要功能是向现有的列表中添加一个新的起始元素，并返回新的列表。
>
> 具体步骤如下：
>
> 1. 使用UF_MODL_create_list() API创建一个新的列表，并返回其指针给new_list。
> 2. 设置new_list的eid为新的起始元素new_start。
> 3. 将new_list的next指向旧的列表old_list，这样就实现了向旧列表添加一个新的起始元素。
> 4. 最后返回新的列表new_list。
>
> 通过这个函数，可以方便地向NX列表中添加新的元素，实现列表的扩展。
>
