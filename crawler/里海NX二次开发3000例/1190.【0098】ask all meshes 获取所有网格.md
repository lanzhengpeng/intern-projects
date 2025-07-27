### 【0098】ask all meshes 获取所有网格

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
    /*  
        UF_SF_locate_all_meshes does not seem to work - see PR 6407606.  This  
        example function can be used to work around.  
    */  
    static int ask_all_meshes(tag_t **meshes)  
    {  
        tag_t  
            part = UF_ASSEM_ask_work_part();  
        return (ask_all_of_type_and_subtype(part,  UF_sfem_mesh_type, 0, meshes));  
    }

```

#### 代码解析

> 这段代码是用于NX Open C++ API的二次开发，主要用于获取当前工作部件中的所有网格对象。
>
> 主要功能包括：
>
> 1. 内存分配函数：allocate_memory，用于分配指定大小的内存。
> 2. 创建数组函数：make_an_array，用于将链表中的对象ID转换为数组。
> 3. 查询对象函数：ask_all_of_type_and_subtype，用于查询当前部件中指定类型和子类型的所有对象，并将对象ID存储在数组中。
> 4. 查询所有网格函数：ask_all_meshes，用于获取当前工作部件中的所有网格对象。
> 5. 在获取网格对象时，由于UF_SF_locate_all_meshes函数存在缺陷，所以使用自定义的ask_all_of_type_and_subtype函数来替代。
> 6. 使用链表来存储查询到的对象，然后转换为数组返回。
> 7. 通过NX Open C++ API实现对NX的二次开发。
>
> 综上所述，这段代码实现了在NX部件中查询指定类型和子类型的对象，并以数组形式返回，为后续的网格处理提供了基础。
>
