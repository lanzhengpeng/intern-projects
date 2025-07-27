### 【1856】remove duplicate tags from array 从数组中移除重复标签

#### 代码

```cpp
    static uf_list_p_t make_a_list(int count, tag_t *item_array)  
    {  
        int  
            ii;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        for (ii = 0; ii < count; ii++)  
            UF_CALL(UF_MODL_put_list_item(list, item_array[ii]));  
        return (list);  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static int remove_duplicate_tags_from_array(int n_objects, tag_t **objects)  
    {  
        uf_list_p_t  
            list = make_a_list(n_objects, *objects);  
        UF_free(*objects);  
        return make_an_array(&list, objects);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，用于处理NX中的实体标签数组，主要包含以下功能：
>
> 1. make_a_list函数：根据提供的标签数组创建一个NX实体列表。
> 2. allocate_memory函数：分配指定大小的内存。
> 3. make_an_array函数：将NX实体列表转换成标签数组。
> 4. remove_duplicate_tags_from_array函数：移除标签数组中的重复标签，并返回移除重复标签后的数组。
>
> 通过这些函数，代码实现了NX实体列表与标签数组之间的转换，并能够移除重复标签，为后续处理提供了便利。
>
