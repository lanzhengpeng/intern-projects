### 【0496】close all other parts 关闭所有其他部件

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
    static logical add_proto_to_list(tag_t comp, void *user_data)  
    {  
        tag_t  
            proto = UF_ASSEM_ask_prototype_of_occ(comp);  
        uf_list_p_t  
            *list = (uf_list_p_t *)user_data;  
        UF_CALL(UF_MODL_put_list_item(*list, proto));  
        return TRUE;  
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
    static int ask_all_assembly_parts(tag_t part, tag_t **parts)  
    {  
        tag_t  
            cset;  
        uf_list_p_t  
            part_list;  
        UF_PART_load_status_t  
            status;  
        UF_CALL(UF_MODL_create_list(&part_list));  
        UF_CALL(UF_MODL_put_list_item(part_list, part));  
        UF_CALL(UF_ASSEM_ask_all_comp_cset(part, &cset));  
        if (cset != NULL_TAG)  
        {  
            UF_CALL(UF_PART_open_cset(cset, &status));  
            report_load_status(&status);  
            UF_CALL(UF_ASSEM_apply_to_cset(cset, add_proto_to_list, &part_list));  
        }  
        return make_an_array(&part_list, parts);  
    }  
    static void close_all_other_parts(tag_t dontClose)  
    {  
        int  
            ii,  
            n_parts;  
        tag_t  
            *parts;  
        uf_list_p_t  
            parts_list;  
        if ((n_parts = UF_PART_ask_num_parts()) == 0) return;  
        UF_CALL(UF_MODL_create_list(&parts_list));  
        for (ii = 0; ii < n_parts; ii++)  
            UF_CALL(UF_MODL_put_list_item(parts_list, UF_PART_ask_nth_part(ii)));  
        n_parts = ask_all_assembly_parts(dontClose, &parts);  
        for (ii = 0; ii < n_parts; ii++)  
            UF_CALL(UF_MODL_delete_list_item(&parts_list, parts[ii]));  
        if (n_parts > 0) UF_free(parts);  
        n_parts = make_an_array(&parts_list, &parts);  
        for (ii = 0; ii < n_parts; ii++)  
            UF_CALL(UF_PART_close(parts[ii], 0, 1));  
        if (n_parts > 0) UF_free(parts);  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能是关闭所有打开的部件，除了指定的一个部件。具体包括以下几个关键部分：
>
> 1. allocate_memory：动态分配内存的函数。
> 2. make_an_array：将列表转换为数组。
> 3. add_proto_to_list：将组件的原型添加到列表中。
> 4. report_load_status：报告部件加载状态。
> 5. ask_all_assembly_parts：获取所有组件的部件，并返回部件数组。
> 6. close_all_other_parts：关闭除指定部件外的所有其他部件。
>
> 首先，它会获取当前所有打开的部件，然后获取指定部件的所有组件，将这些组件的原型部件添加到一个列表中。接着，它会关闭除指定部件外的所有其他部件，最后释放内存。通过这个函数，可以确保在操作指定部件时，其他部件不会影响当前的操作。
>
