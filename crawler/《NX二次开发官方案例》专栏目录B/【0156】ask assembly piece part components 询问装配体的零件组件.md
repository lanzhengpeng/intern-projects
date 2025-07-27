### 【0156】ask assembly piece part components 询问装配体的零件组件

#### 代码

```cpp
    static void add_piece_part_comps_to_list(tag_t comp, uf_list_p_t pp_list)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *children;  
        if ((n = UF_ASSEM_ask_all_part_occ_children(comp, &children)) == 0)  
        {  
            UF_CALL(UF_MODL_put_list_item(pp_list, comp));  
        }  
        else  
        {  
            for (ii = 0; ii < n; ii++)  
                add_piece_part_comps_to_list(children[ii], pp_list);  
            UF_free(children);  
        }  
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
    /*  This will return the lowest level piece part components in an assembly  
        even if no components are loaded.  */  
    static int ask_assembly_piece_part_components(tag_t assy, tag_t **pp_comps)  
    {  
        tag_t  
            root = UF_ASSEM_ask_root_part_occ(assy);  
        uf_list_p_t  
            pp_list;  
        UF_CALL(UF_MODL_create_list(&pp_list));  
        add_piece_part_comps_to_list(root, pp_list);  
        return make_an_array(&pp_list, pp_comps);  
    }

```

#### 代码解析

> 这段代码实现了以下功能：
>
> 1. 递归遍历组件：add_piece_part_comps_to_list函数递归地遍历装配体组件树，将所有最低级别的零件组件添加到一个链表中。
> 2. 内存分配：allocate_memory函数用于分配内存。
> 3. 创建数组：make_an_array函数将链表转换为数组，然后释放链表。
> 4. 获取零件组件：ask_assembly_piece_part_components函数首先获取装配体的根组件，然后调用add_piece_part_comps_to_list函数获取所有零件组件，最后调用make_an_array函数将链表转换为数组并返回。
>
> 整体来说，这段代码的目的是获取NX装配体中的所有零件组件，并将其以数组的形式返回。通过递归遍历组件树，它可以找到所有最低级别的零件组件，并处理了内存分配和链表操作等细节。
>
