### 【0037】ask all objects on work layer 在工作层上查询所有对象

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
    static int ask_all_objects_on_work_layer(tag_t **objects)  
    {  
        int  
            work;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        UF_CALL(UF_LAYER_ask_work_layer(&work));  
        while (!UF_CALL(UF_LAYER_cycle_by_layer(work, &object)) &&  
            (object != NULL_TAG)) UF_CALL(UF_MODL_put_list_item(list, object));  
        return make_an_array(&list, objects);  
    }

```

#### 代码解析

> 根据代码，这是一个用于NX Open C++ API的二次开发代码段，其主要功能是获取当前工作层上的所有对象，并存储在tag_t类型的数组中。以下是代码的详细解释：
>
> 1. allocate_memory函数：该函数用于在NX环境中分配内存，参数nbytes指定要分配的字节数，参数where用于存放分配的内存地址，返回值为响应码。该函数通过调用UF_allocate_memory API在NX环境中分配内存。
> 2. make_an_array函数：该函数用于将UF_list_p_t类型的链表转换为tag_t类型的数组。首先获取链表中的对象数量，然后分配相应大小的内存，遍历链表将对象ID存入数组，最后删除链表。
> 3. ask_all_objects_on_work_layer函数：该函数用于获取当前工作层上的所有对象。首先创建一个UF_list_p_t类型的链表，然后获取当前工作层，并遍历工作层上的所有对象，将对象的ID存入链表中。最后调用make_an_array函数将链表转换为tag_t数组。
>
> 总体来说，这段代码实现了在NX中获取当前工作层所有对象的功能，并提供了两种数据结构（链表和数组）来存储这些对象，用户可以根据需要选择使用哪种数据结构。
>
