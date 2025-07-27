### 【0019】ask all faces 问所有面

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
    static int ask_all_of_type_and_subtype(tag_t part, int type, int subtype,  
        tag_t **objects)  
    {  
        int  
            this_type,  
            this_subtype;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object)) &&  
                (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &this_type, &this_subtype));  
            if (this_subtype == subtype)  
                (UF_MODL_put_list_item(object_list, object));  
        }  
        return (make_an_array(&object_list, objects));  
    }  
    static int ask_all_faces(tag_t part, tag_t **faces)  
    {  
        return ask_all_of_type_and_subtype(part, UF_solid_type,  
            UF_solid_face_subtype, faces);  
    }

```

#### 代码解析

> 这段代码是NX Open API的二次开发代码，主要实现了以下功能：
>
> 1. allocate_memory函数：用于分配指定大小的内存空间，并返回内存地址。
> 2. make_an_array函数：从对象列表中创建一个数组，存储列表中所有对象的实体标签，并删除对象列表。
> 3. ask_all_of_type_and_subtype函数：遍历部件中的所有指定类型和子类型对象，将满足条件的对象添加到列表中，然后调用make_an_array函数创建对象数组。
> 4. ask_all_faces函数：获取部件中的所有面，通过调用ask_all_of_type_and_subtype函数实现。
>
> 这些函数主要用于NX Open API的二次开发，以便遍历和操作NX模型中的对象。它们通过UF_MODL和UF_OBJ提供的API来实现，从而提高了代码的复用性和可维护性。
>
