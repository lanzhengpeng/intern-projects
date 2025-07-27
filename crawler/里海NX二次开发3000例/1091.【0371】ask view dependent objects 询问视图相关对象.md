### 【0371】ask view dependent objects 询问视图相关对象

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
    static int ask_view_dependent_objects(char *view, tag_t **objects)  
    {  
        tag_t  
            an_object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while (!UF_CALL(uc6457(view, 2, &an_object)) && (an_object != NULL_TAG))  
            UF_CALL(UF_MODL_put_list_item(object_list, an_object));  
        return (make_an_array(&object_list, objects));  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的NX二次开发代码，主要用于查询视图依赖的图元对象。
>
> 主要功能包括：
>
> 1. allocate_memory函数：用于申请指定大小的内存，返回内存地址。
> 2. make_an_array函数：从UF_MODL_ask_list_count获取列表数量n，然后申请n个tag_t大小的内存，将列表中的元素复制到数组中，最后删除列表。
> 3. ask_view_dependent_objects函数：用于查询视图view的依赖对象，并将它们存储在数组中返回。
>
> 函数的调用流程如下：
>
> 1. 调用allocate_memory申请内存，用于存储查询结果。
> 2. 调用UF_MODL_create_list创建一个空列表，用于存储查询到的对象。
> 3. 循环调用uc6457函数查询视图的依赖对象，并调用UF_MODL_put_list_item将对象添加到列表中。
> 4. 调用make_an_array将列表中的对象复制到数组中，并删除列表。
> 5. 最后返回数组的首地址。
>
> 通过这段代码，可以实现查询视图依赖的图元对象，并返回这些对象。
>
