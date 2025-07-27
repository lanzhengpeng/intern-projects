### 【0049】ask all reference sets 询问所有参考集

#### 代码

```cpp
    static tag_t ask_next_ref_set(tag_t part, tag_t refset)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_reference_set_type,  
            &refset)) && (refset != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(refset, &type, &subtype));  
            if ((subtype == UF_reference_design_subtype) ||  
                (subtype == UF_reference_design_model_subtype) ||  
                (subtype == UF_reference_design_lightweight_subtype))  
                return refset;  
        }  
        return NULL_TAG;  
    }  
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
    static int ask_all_reference_sets(tag_t part, tag_t **refsets)  
    {  
        tag_t  
            refset = NULL_TAG;  
        uf_list_p_t  
            refset_list;  
        UF_CALL(UF_MODL_create_list(&refset_list));  
        while ((refset = ask_next_ref_set(part, refset)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(refset_list, refset));  
        return (make_an_array(&refset_list, refsets));  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，用于遍历NX部件中的所有参考集。
>
> 主要功能如下：
>
> 1. ask_next_ref_set函数：用于遍历并返回下一个参考集，直到遍历完所有参考集为止。它通过调用UF_OBJ_cycle_objs_in_part函数来遍历部件中的所有对象，并检查其类型是否为参考集。
> 2. allocate_memory函数：用于分配指定大小的内存空间。
> 3. make_an_array函数：用于将链表转换为数组。它首先获取链表中的元素数量，然后分配对应大小的内存，并将链表中的元素复制到数组中。
> 4. ask_all_reference_sets函数：首先创建一个空链表，然后调用ask_next_ref_set函数遍历并收集所有参考集，将其添加到链表中。最后调用make_an_array函数将链表转换为数组，并返回数组的大小。
>
> 总体而言，这段代码实现了遍历NX部件中所有参考集的功能，并使用数组来存储这些参考集，以便后续处理。
>
