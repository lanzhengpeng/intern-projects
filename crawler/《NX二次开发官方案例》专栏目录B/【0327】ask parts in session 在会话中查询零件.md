### 【0327】ask parts in session 在会话中查询零件

#### 代码

```cpp
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
    static int ask_parts_in_session(tag_t **parts)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            part;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        n = UF_PART_ask_num_parts();  
        for (ii = 0; ii < n; ii++)  
        {  
            part = UF_PART_ask_nth_part(ii);  
            UF_CALL(UF_MODL_put_list_item(list, part));  
        }  
        return (make_an_array(&list, parts));  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++应用程序，用于查询当前会话中的所有部件并获取它们的tag。
>
> 主要步骤包括：
>
> 1. 使用UF_MODL_create_list()创建一个空列表list。
> 2. 使用UF_PART_ask_num_parts()获取当前会话中的部件数量n。
> 3. 使用一个循环，遍历所有部件，并使用UF_PART_ask_nth_part(ii)获取每个部件的tag，然后使用UF_MODL_put_list_item()将每个部件的tag添加到列表list中。
> 4. 调用make_an_array()函数，该函数执行以下操作：获取列表list中的元素数量。为部件tag数组分配内存。遍历列表list，将每个部件的tag复制到数组中。删除列表list。返回数组的大小。
> 5. 获取列表list中的元素数量。
> 6. 为部件tag数组分配内存。
> 7. 遍历列表list，将每个部件的tag复制到数组中。
> 8. 删除列表list。
> 9. 返回数组的大小。
> 10. ask_parts_in_session()函数返回部件tag数组的大小。
>
> 综上，该代码实现了查询当前会话中的所有部件并获取它们的tag的功能。
>
