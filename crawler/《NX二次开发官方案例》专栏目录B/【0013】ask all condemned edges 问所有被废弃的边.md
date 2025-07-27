### 【0013】ask all condemned edges 问所有被废弃的边

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
    static int ask_all_condemned_objects_of_type_and_subtype(tag_t part, int type,  
        int subtype, tag_t **objects)  
    {  
        int  
            this_type,  
            this_subtype;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while ((object = UF_OBJ_cycle_all(part, object)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &this_type, &this_subtype));  
            if ((this_type == type) && (this_subtype == subtype) &&  
                (UF_OBJ_ask_status(object) == UF_OBJ_CONDEMNED))  
                    UF_MODL_put_list_item(object_list, object);  
        }  
        return (make_an_array(&object_list, objects));  
    }  
    static int ask_all_condemned_edges(tag_t part, tag_t **edges)  
    {  
        return ask_all_condemned_objects_of_type_and_subtype(part, UF_solid_type,  
            UF_solid_edge_subtype, edges);  
    }

```

#### 代码解析

> 根据这段代码，它主要实现了以下功能：
>
> 1. 内存分配：allocate_memory函数用于向 NX 二次开发环境申请指定大小的内存空间。
> 2. 创建数组：make_an_array函数根据对象列表创建一个数组，存储对象的 ID，并删除对象列表。
> 3. 查询被标记删除的对象：ask_all_condemned_objects_of_type_and_subtype函数用于查询指定类型和子类型的被标记删除的对象，并将这些对象放入一个列表中。
> 4. 查询所有被标记删除的边：ask_all_condemned_edges函数调用上述函数，用于查询指定部件中所有被标记删除的边，并将这些边的 ID 存储在数组中。
>
> 总的来说，这段代码的主要功能是查询被标记删除的对象，并返回这些对象的 ID 数组，主要用于 NX 二次开发中的数据处理。
>
