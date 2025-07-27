### 【1029】delete objects of object list 删除对象列表中的对象

#### 代码

```cpp
    static void delete_objects_of_object_list(uf_list_p_t list)  
    {  
        uf_list_p_t  
            tmp;  
        for (tmp = list; tmp != NULL; tmp = tmp->next)  
            UF_CALL(UF_OBJ_delete_object(tmp->eid));  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API的二次开发代码，其功能是遍历一个对象链表，并删除其中的每一个对象。
>
> 具体来说，代码定义了一个名为delete_objects_of_object_list的静态函数，接受一个指向对象链表头部的指针作为参数。在这个函数中，通过一个for循环遍历整个链表。在每次循环中，通过UF_OBJ_delete_object函数删除链表当前节点的对象。循环结束后，整个链表中的所有对象都被成功删除。
>
> 该函数通过UF_CALL宏来调用UF_OBJ_delete_object函数，以确保函数调用的正确性。同时，该函数使用了NX Open C++ API中的链表数据结构，以实现对象的高效管理。
>
> 总体来说，这段代码实现了在NX Open C++环境中批量删除对象的功能，为NX的二次开发提供了便利。
>
