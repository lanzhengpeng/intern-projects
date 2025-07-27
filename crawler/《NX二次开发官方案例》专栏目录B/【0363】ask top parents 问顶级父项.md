### 【0363】ask top parents 问顶级父项

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
    static int ask_top_parents(tag_t child, tag_t **top_parents)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *parents;  
        uf_list_p_t  
            rel,  
            rel_list,  
            top_list;  
        UF_CALL(UF_MODL_create_list(&rel_list));  
        UF_CALL(UF_MODL_create_list(&top_list));  
        UF_CALL(UF_MODL_put_list_item(rel_list, child));  
        for (rel = rel_list; rel != NULL; rel = rel->next)  
        {  
            n = UF_ASSEM_where_is_part_used(rel->eid, &parents);  
            if (n == 0)  
                UF_CALL(UF_MODL_put_list_item(top_list, rel->eid));  
            else  
            {  
                for (ii = 0; ii < n; ii++)  
                    UF_CALL(UF_MODL_put_list_item(rel_list, parents[ii]));  
                UF_free(parents);  
            }  
        }  
        UF_CALL(UF_MODL_delete_list(&rel_list));  
        return make_an_array(&top_list, top_parents);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的NX二次开发代码，其主要功能是查询一个NX部件的所有顶级父部件。
>
> 代码的主要流程如下：
>
> 1. allocate_memory函数用于申请指定大小的内存，并返回内存地址。
> 2. make_an_array函数用于将一个NX列表转换为tag数组，并删除原始列表。
> 3. ask_top_parents函数是主函数，用于查询指定部件的所有顶级父部件。
> 4. 在ask_top_parents函数中，首先创建两个NX列表rel_list和top_list。
> 5. 将传入的child部件放入rel_list列表中。
> 6. 遍历rel_list列表，对于每个部件，调用UF_ASSEM_where_is_part_used函数查询其所有父部件，并将父部件放入rel_list列表。
> 7. 如果父部件没有父部件，则放入top_list列表。
> 8. 遍历完成后，删除rel_list列表，并调用make_an_array函数将top_list列表转换为tag数组。
> 9. 返回顶级父部件的tag数组。
>
> 这段代码通过递归查询，实现了查询一个NX部件的所有顶级父部件的功能。
>
