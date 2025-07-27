### 【1571】make an array make_an_array函数将UF_MODL_ask_list_count得到的列表数量n作为数组大小，动态分配内存并创建一个tag_t类型的数组接着，通过

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

```

#### 代码解析

> 这段代码是NX Open C++ API编写的二次开发代码，其主要功能是分配内存并创建一个数组来存储NX模型中对象的ID。
>
> 代码主要包含两个静态函数：
>
> 1. allocate_memory: 该函数用于分配指定大小的内存，并将内存地址存储在where参数指向的指针中。函数通过调用UF_allocate_memory API来实现内存分配，并返回API调用的结果。
> 2. make_an_array: 该函数用于根据传入的对象列表创建一个存储对象ID的数组。首先，它调用UF_MODL_ask_list_count来获取对象列表的数量，然后调用allocate_memory来为ID数组分配内存。接着，通过遍历对象列表，将每个对象的ID存储到数组中。最后，调用UF_MODL_delete_list来删除传入的对象列表，并返回数组的大小。
>
> 总的来说，这段代码通过调用NX Open API，实现了内存分配和对象ID数组创建的功能，是典型的NX二次开发代码。
>
