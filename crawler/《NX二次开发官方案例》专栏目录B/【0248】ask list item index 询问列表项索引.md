### 【0248】ask list item index 询问列表项索引

#### 代码

```cpp
    static logical ask_list_item_index(uf_list_p_t object_list, tag_t item,  
        int *index)  
    {  
        int  
            ii;  
        uf_list_p_t  
            temp;  
        for (ii = 0, temp = object_list; temp != NULL; temp = temp->next, ii++)  
            if (temp->eid == item) break;  
        if (temp == NULL) return FALSE;  
        *index = ii;  
        return TRUE;  
    }

```

#### 代码解析

> 这段代码是用于在NX Open的二次开发中，实现查询列表中元素索引的功能。具体来说：
>
> 1. 函数名为ask_list_item_index，其参数包括列表头指针object_list、元素标识item和索引指针index。
> 2. 使用了两个局部变量：ii用于记录索引，temp用于遍历列表。
> 3. 通过一个for循环遍历整个列表，同时使用ii来记录索引。
> 4. 在循环中，如果找到了与item匹配的元素，就跳出循环。
> 5. 如果没有找到匹配的元素，返回FALSE。
> 6. 如果找到了匹配的元素，将索引ii赋值给index，并返回TRUE。
>
> 总体而言，这段代码实现了在列表中查找元素并返回其索引的功能，是一个在NX Open二次开发中常用的功能函数。
>
