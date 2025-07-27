### 【0190】ask edge faces 询问边关联的面

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
    static int ask_edge_faces(tag_t edge, tag_t **faces)  
    {  
        uf_list_p_t   
            facelist;  
        UF_CALL(UF_MODL_ask_edge_faces(edge, &facelist));  
        return (make_an_array(&facelist, faces));  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API开发的示例，用于获取NX中一个边所关联的所有面的列表。代码的主要功能包括：
>
> 1. allocate_memory函数：该函数用于在NX中分配指定大小的内存，并返回分配的内存指针。
> 2. make_an_array函数：该函数用于将NX中的对象列表转换成C++中的tag_t数组。首先，它会获取对象列表的长度，然后分配相应大小的内存，并将对象列表中的对象依次复制到数组中。最后，它会删除NX中的对象列表。
> 3. ask_edge_faces函数：该函数用于获取给定边关联的所有面的列表。它首先调用NX API获取边的面的列表，然后调用make_an_array函数将列表转换为C++中的tag_t数组，并返回数组的长度。
> 4. 主函数：首先获取一个边的tag，然后调用ask_edge_faces函数获取该边关联的所有面的tag数组，最后打印出面的tag数组。
> 5. NX Open C++ API：该代码使用了NX Open C++ API，这是NX的二次开发接口，提供了丰富的函数来操作NX模型。
>
> 总的来说，这段代码实现了在NX中查询边关联的所有面，并将结果转换为C++数组的功能，为NX二次开发提供了一个典型的示例。
>
