### 【3114】find tags from handles 根据句柄查找标记，返回标记数组

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
    static int find_tags_from_handles(unsigned int n_handles, char **handles,  
        tag_t **objects)  
    {  
        unsigned int  
            ii, jj,  
            orig_version,  
            sub_file_id,  
            this_version;  
        tag_t  
            object,  
            point;  
        double  
            zero[3] = { 0,0,0 };  
        char  
            *handle,  
            *orig_file,  
            *this_file;  
        uf_list_p_t  
            object_list;  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark_id));  
        UF_CALL(UF_CURVE_create_point(zero, &point));  
        handle = UF_TAG_ask_handle_of_tag(point);  
        UF_CALL(UF_TAG_decompose_handle(handle, &this_file, &sub_file_id,  
            &this_version));  
        UF_free(handle);  
        UF_CALL(UF_UNDO_undo_to_mark(mark_id, NULL));  
        UF_CALL(UF_UNDO_delete_mark(mark_id, NULL));  
        for (ii = 0; ii < n_handles; ii++)  
        {  
            UF_CALL(UF_TAG_decompose_handle(handles[ii], &orig_file, &sub_file_id,  
                &orig_version));  
            for (jj = orig_version; jj <= this_version; jj++)  
            {  
                UF_CALL(UF_TAG_compose_handle(this_file, sub_file_id, jj,  
                    &handle));  
                if ((object = UF_TAG_ask_tag_of_handle(handle)) != NULL_TAG)  
                {  
                    UF_CALL(UF_MODL_put_list_item(object_list, object));  
                    UF_free(handle);  
                    break;  
                }  
                UF_free(handle);  
            }  
            UF_free(orig_file);  
        }  
        UF_free(this_file);  
        return make_an_array(&object_list, objects);  
    }

```

#### 代码解析

> 这段NX Open C++代码主要实现了以下几个功能：
>
> 1. allocate_memory函数：用于在NX中分配指定大小的内存，并将内存地址存储在指定的void**指针中。
> 2. make_an_array函数：接受一个UF_MODL_list结构体指针和一个tag_t指针作为参数。它会遍历list中的所有元素，将它们的tag_t存储在tag_t数组中，然后释放list所占的内存。
> 3. find_tags_from_handles函数：接受多个句柄字符串，根据这些句柄字符串查找对应的NX对象，并将这些对象存储在一个tag_t**数组中返回。
> 4. 主要流程：该函数首先创建一个UF_MODL_list，用于存放找到的对象。然后，它遍历所有输入的句柄，根据句柄的版本号，尝试查找对应的NX对象，并将找到的对象添加到list中。最后，调用make_an_array函数将list中的对象存储到返回的tag_t**数组中。
> 5. 整体功能：该代码实现了根据句柄字符串查找对应NX对象的功能，并以tag_t数组的形式返回这些对象。它考虑了对象在不同版本中的存在性，并能够处理多个句柄输入。
>
> 这段代码利用了NX Open C++ API，实现了对象查找和内存管理等功能，为NX的二次开发提供了实用功能。
>
