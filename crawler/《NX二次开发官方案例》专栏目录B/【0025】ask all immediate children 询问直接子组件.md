### 【0025】ask all immediate children 询问直接子组件

#### 代码

```cpp
    /*  
        This function is basically the same as UF_ASSEM_ask_part_occ_children  
        except that suppressed components are included.  
    */  
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
    static int ask_all_immediate_children(tag_t part, tag_t **children)  
    {  
        tag_t  
            comp = NULL_TAG,  
            inst,  
            parent;  
        uf_list_p_t  
            child_list;  
        UF_CALL(UF_MODL_create_list(&child_list));  
        while ((comp = ask_next_component(part, comp)) != NULL_TAG)  
        {  
            if ((inst = UF_ASSEM_ask_inst_of_part_occ(comp)) != NULL_TAG)  
            {  
                parent = UF_ASSEM_ask_parent_of_instance(inst);  
                if (parent == part)  
                    UF_CALL(UF_MODL_put_list_item(child_list, comp));  
            }  
        }  
        return (make_an_array(&child_list, children));  
    }

```

#### 代码解析

> 根据代码，这是一段用于NX Open C++ API的二次开发代码，主要实现了以下功能：
>
> 1. allocate_memory函数：用于分配指定大小的内存，并将内存地址存储在where指向的指针中。
> 2. make_an_array函数：接收一个组件列表object_list，创建一个组件数组objects，并将列表中的组件ID复制到数组中，然后释放列表占用的内存。
> 3. ask_all_immediate_children函数：获取指定组件part的所有直接子组件，并将子组件的ID存储在children数组中返回。
> 4. 使用UF_MODL_create_list创建一个列表child_list，用于存储part的所有直接子组件。
> 5. 通过ask_next_component函数遍历part的所有组件，并判断组件是否为实例。如果是实例，则获取其实例的父组件，如果父组件是part，则将组件添加到child_list中。
> 6. 调用make_an_array函数，将child_list中的组件ID复制到children数组中，并释放child_list占用的内存。
> 7. 最后返回children数组的长度。
>
> 这段代码主要用于获取组件的所有直接子组件，并存储在数组中返回。
>
