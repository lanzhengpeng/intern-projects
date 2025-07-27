### ask all geometry 询问所有几何对象

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
    static int ask_all_geometry(tag_t **geometry)  
    {  
        logical  
            ok;  
        int  
            subtype,  
            type;  
        tag_t  
            item = NULL_TAG;  
        uf_list_p_t  
            geo_list;  
        UF_CALL(UF_MODL_create_list(&geo_list));  
        while (!UF_CALL(UF_LAYER_cycle_by_layer(0, &item)) && (item != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(item, &type, &subtype));  
            UF_CALL(UF_OBJ_is_type_transferable(type, &ok));  
            if (ok) UF_CALL(UF_MODL_put_list_item(geo_list, item));  
        }  
        return make_an_array(&geo_list, geometry);  
    }

```

```cpp
static int allocate_memory(unsigned int nbytes, void **where)
{
    int resp;
    *where = UF_allocate_memory(nbytes, &resp);
    return resp;
}

```

```cpp
static int make_an_array(uf_list_p_t *object_list, tag_t **objects)
{
    // ...
}

```

```cpp
static int ask_all_geometry(tag_t **geometry)
{
    // ...
}

```

#### 代码解析

> 这段代码是用C++编写的，用于NX（一种CAD/CAM/CAE软件）的二次开发。代码的核心功能是检索当前NX会话中所有可转换的层（layer）的几何对象，并将这些对象的标签（tag）收集到一个数组中。
>
> 以下是代码的逐部分介绍：
>
> ```cpp
> static int allocate_memory(unsigned int nbytes, void **where)
> {
>     int resp;
>     *where = UF_allocate_memory(nbytes, &resp);
>     return resp;
> }
> ```
>
> 这个辅助函数用于分配指定大小的内存。它调用NX提供的UF_allocate_memory函数，该函数在NX的API中用于动态分配内存。函数接收所需字节数和指向void*指针的指针（where），以存储内存分配的地址。成功时返回0，否则返回错误代码。
>
> ```cpp
> static int make_an_array(uf_list_p_t *object_list, tag_t **objects)
> {
>     // ...
> }
> ```
>
> 这个函数用于从NX列表中获取对象标签，并创建一个标签数组。函数首先获取列表中的对象数量，然后使用allocate_memory函数分配足够的空间来存储标签。之后，它遍历列表，将每个对象的标签复制到新创建的数组中，并最终删除NX列表。
>
> ```cpp
> static int ask_all_geometry(tag_t **geometry)
> {
>     // ...
> }
> ```
>
> 这个函数是这段代码的主要功能。它创建一个列表，用于存储所有可转换的几何对象。函数通过遍历所有层，检查每个层的对象是否可以转换，并收集那些可转换的对象。
>
> 以下是详细步骤：
>
> 1. 创建一个空列表geo_list用于存储可转换的几何对象标签。
> 2. 使用UF_LAYER_cycle_by_layer函数遍历所有层。该函数在每次调用时会返回当前层的对象标签，直到所有层被遍历完成。
> 3. 对于每个对象，使用UF_OBJ_ask_type_and_subtype函数获取对象的类型和子类型。
> 4. 使用UF_OBJ_is_type_transferable函数检查对象是否可转换。
> 5. 如果对象可转换，将对象添加到geo_list中。
> 6. 当层遍历完成后，调用make_an_array函数，将列表中的对象标签转换为数组。
>
> 代码中出现的UF_CALL宏可能是用于简化对NX API函数调用的错误处理。通常，它会检查返回值并报告错误（如果有的话）。
>
> 整体来说，这段代码是NX二次开发的一部分，用于提取几何数据，这可能是为了进一步处理、分析或转换。
>
