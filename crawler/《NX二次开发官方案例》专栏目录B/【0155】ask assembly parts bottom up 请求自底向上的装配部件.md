### 【0155】ask assembly parts bottom up 请求自底向上的装配部件

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
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            UF_UI_open_listing_window();  
            UF_UI_write_listing_window("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_UI_write_listing_window("  ");  
                UF_UI_write_listing_window(status->file_names[ii]);  
                UF_UI_write_listing_window(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window("\n");  
            }  
            UF_UI_write_listing_window("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static int ask_assembly_parts_bottom_up(tag_t top, tag_t **children)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            child,  
            *child_occs,  
            inst,  
            root;  
        uf_list_p_t  
            front,  
            the_list,  /* The bottom up ordered list 译:底部向上顺序列表 */  
            tmp_list,  /* Used to walk all the assembly parts 译:Used to walk all the assembly parts 翻译为：曾经走遍所有的装配车间。 */  
            tmp;  
        UF_PART_load_status_t  
                load_status;  
        UF_CALL(UF_MODL_create_list(&the_list));  
        UF_CALL(UF_MODL_put_list_item(the_list, top));  
        UF_CALL(UF_MODL_create_list(&tmp_list));  
        UF_CALL(UF_MODL_put_list_item(tmp_list, top));  
        for (tmp = tmp_list; tmp != NULL; tmp = tmp->next)  
        {  
            root = UF_ASSEM_ask_root_part_occ(tmp->eid);  
            if (root == NULL_TAG) continue;  
            if ((n = UF_ASSEM_ask_part_occ_children(root, &child_occs)) > 0)  
            {  
                for (ii = 0; ii < n; ii++)  
                {  
                    inst = UF_ASSEM_ask_inst_of_part_occ(child_occs[ii]);  
                    if (inst == NULL_TAG) continue;  
                    UF_CALL(UF_ASSEM_ensure_child_loaded(inst, &load_status));  
                    report_load_status(&load_status);  
                    child = UF_ASSEM_ask_prototype_of_occ(child_occs[ii]);  
                    if (child == NULL_TAG) continue;  
                    UF_CALL(UF_MODL_put_list_item(tmp_list, child));  
                /*  Put the children on the FRONT of the ordered list 译:把孩子们放在预定名单的前面。 */  
                    UF_CALL(allocate_memory(sizeof(uf_list_p_t), (void **)&front));  
                    front->eid = child;  
                    front->next = the_list;  
                    the_list = front;  
                }  
                UF_free(child_occs);  
            }  
        }  
        UF_CALL(UF_MODL_delete_list(&tmp_list));  
    /*  Remove the duplicates from the ordered list maintaining order 译:从已排序的列表中移除重复项，保持顺序。 */  
        tmp_list = the_list;  
        UF_CALL(UF_MODL_create_list(&the_list));  
        for (tmp = tmp_list; tmp != NULL; tmp = tmp->next)  
            UF_CALL(UF_MODL_put_list_item(the_list, tmp->eid));  
        return make_an_array(&the_list, children);  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发的功能，主要实现了以下功能：
>
> 1. 内存分配：通过调用UF_allocate_memory函数来分配内存，并将分配的内存地址存放在where参数指向的指针中。
> 2. 创建数组：通过遍历零件列表，将列表中的零件ID存入一个tag_t类型的数组中，并返回数组的长度。
> 3. 报告加载状态：通过打开一个窗口，并写入加载失败零件的信息，以便报告零件加载的状态。
> 4. 自底向上询问装配零件：通过递归遍历装配树，自底向上获取所有装配零件的ID，并将它们按顺序存入一个数组中。在遍历过程中，确保每个零件都已加载，并处理加载失败的情况。
> 5. 移除重复项：在获取到所有零件ID后，移除重复的ID，只保留唯一的零件ID。
>
> 总的来说，这段代码实现了NX装配树的自底向上遍历，获取所有零件的ID，并进行了去重处理，以便后续进行其他操作。
>
