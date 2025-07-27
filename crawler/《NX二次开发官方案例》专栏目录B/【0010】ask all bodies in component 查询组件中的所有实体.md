### 【0010】ask all bodies in component 查询组件中的所有实体

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
    static void recursively_dig_for_bodies(tag_t component, uf_list_p_t body_list)  
    {  
        int  
            subtype,  
            type;  
        tag_t  
            obj = NULL_TAG;  
        do {  
            obj = UF_ASSEM_cycle_ents_in_part_occ(component, obj);  
            if (obj == NULL_TAG) break;  
            UF_CALL(UF_OBJ_ask_type_and_subtype(obj, &type, &subtype));  
            if ((type == UF_solid_type) && (subtype == UF_solid_body_subtype))  
                UF_CALL(UF_MODL_put_list_item(body_list, obj));  
            if ((type == UF_component_type) && (subtype == UF_part_occurrence_subtype))  
                recursively_dig_for_bodies(obj, body_list);  
        } while (true);  
    }  
    static int ask_all_bodies_in_component(tag_t component, tag_t **solids)  
    {  
        uf_list_p_t  
            body_list;  
        UF_CALL(UF_MODL_create_list(&body_list));  
        recursively_dig_for_bodies(component, body_list);  
        return (make_an_array(&body_list, solids));  
    }

```

#### 代码解析

> 这段代码是用于NX Open API的二次开发代码，其主要功能是从NX组件中检索所有实体体并返回它们的标签数组。
>
> 关键点如下：
>
> 1. allocate_memory()函数：用于分配内存空间，其中nbytes指定要分配的字节数，where指向分配的内存地址。
> 2. make_an_array()函数：从实体列表中创建一个标签数组，首先获取列表中的实体数量，然后分配相应大小的内存空间，将列表中的实体标签复制到数组中，最后删除列表。
> 3. recursively_dig_for_bodies()函数：递归地检索组件中的所有实体体，首先获取组件中的下一个实体，然后判断其类型，如果是实体体，则添加到实体列表中；如果是子组件，则递归调用该函数。
> 4. ask_all_bodies_in_component()函数：用于获取组件中的所有实体体的标签数组，首先创建一个实体列表，然后递归调用函数检索组件中的所有实体体，最后调用make_an_array()函数生成实体标签数组并返回数组大小。
>
> 综合来看，这段代码通过递归遍历组件，收集其中的所有实体体，并将它们的标签存储在一个数组中返回，为NX二次开发提供了有效的实体检索功能。
>
