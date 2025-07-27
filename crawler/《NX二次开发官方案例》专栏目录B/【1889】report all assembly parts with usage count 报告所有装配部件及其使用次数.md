### 【1889】report all assembly parts with usage count 报告所有装配部件及其使用次数

#### 代码

```cpp
    /*HEAD REPORT_ALL_ASSEMBY_PARTS_WITH_USAGE_COUNT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog is new in V18 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
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
    static logical is_item_on_list(uf_list_p_t object_list, tag_t item)  
    {  
        int  
            ii;  
        uf_list_p_t  
            temp;  
        for (ii = 0, temp = object_list; temp != NULL; temp = temp->next, ii++)  
            if (temp->eid == item) return TRUE;  
        return FALSE;  
    }  
    static int ask_assembly_parts_top_down(tag_t top, tag_t **children)  
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
            the_list,  /* The top down up ordered list 里海译:顶层向下顺序列表 */  
            tmp_list,  /* Used to walk all the assembly parts 里海译:曾经需要走遍所有装配部件。 */  
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
            if ((n = UF_ASSEM_ask_part_occ_children( root, &child_occs)) > 0)  
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
                /*  Move the children to the end of the ordered list 里海译:将孩子们移至有序列表的末尾。 */  
                    if (is_item_on_list(the_list, child))  
                        UF_CALL(UF_MODL_delete_list_item(&the_list, child));  
                    UF_CALL(UF_MODL_put_list_item(the_list, child));  
                }  
                UF_free(child_occs);  
            }  
        }  
        UF_CALL(UF_MODL_delete_list(&tmp_list));  
        return make_an_array(&the_list, children);  
    }  
    static void ask_part_shortname(tag_t part, char *shortname)  
    {  
        logical  
            nxman;  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_PART_ask_part_name(part, partname));  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        if (nxman)  
            UF_CALL(UF_PART_name_for_display(partname, shortname));  
        else  
            UF_CALL(uc4574(partname, 2, shortname));  
    }  
    static int ask_immediate_occs_of_part(tag_t parent, tag_t child, tag_t **i_occs)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *occs,  
            root_occ;  
        uf_list_p_t  
            i_occ_list;  
        UF_CALL(UF_MODL_create_list(&i_occ_list));  
        root_occ = UF_ASSEM_ask_root_part_occ(parent);  
        n = UF_ASSEM_ask_occs_of_part(parent, child, &occs);  
        for (ii = 0; ii < n; ii++)  
        {  
            if (UF_ASSEM_ask_part_occurrence(occs[ii]) == root_occ)  
                UF_CALL(UF_MODL_put_list_item(i_occ_list, occs[ii]));  
        }  
        if (n > 0) UF_free(occs);  
        return make_an_array(&i_occ_list, i_occs);  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static void do_it(void)  
    {  
        int  
            count,  
            ii, jj,  
            n_parts;  
        tag_t  
            *occs,  
            part = UF_PART_ask_display_part(),  
            *parts;  
        char  
            comp_name[MAX_FSPEC_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        n_parts = ask_assembly_parts_top_down(part, &parts);  
        for (ii = 1; ii < n_parts; ii++)    /* 0th is top assembly 里海译:0th is top assembly 的翻译为：0th 是顶级装配。 */  
        {  
            count = UF_ASSEM_ask_occs_of_part(part, parts[ii], &occs);  
            if (count > 0) UF_free(occs);  
            ask_part_shortname(parts[ii], comp_name);  
            sprintf(msg, "%s = %d total:  ", comp_name, count);  
            WRITE(msg);  
            for (jj = 0; jj < ii; jj++)  
            {  
                count = ask_immediate_occs_of_part(parts[jj], parts[ii], &occs);  
                if (count > 0)  
                {  
                    UF_free(occs);  
                    ask_part_shortname(parts[jj], comp_name);  
                    sprintf(msg, "[%d in %s]", count, comp_name);  
                    WRITE(msg);  
                }  
            }  
            WRITE("\n");  
        }  
        if (n_parts > 0) UF_free(parts);  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 根据代码内容，这是一段用于NX的二次开发代码，主要实现了以下功能：
>
> 1. 定义了一个报告错误函数report_error，用于打印出函数调用失败的错误信息。
> 2. 实现了allocate_memory和make_an_array函数，用于申请内存和转换链表到数组。
> 3. 定义了report_load_status函数，用于报告零件加载状态。
> 4. 实现了is_item_on_list函数，用于判断元素是否在链表中。
> 5. 实现了ask_assembly_parts_top_down函数，用于递归获取装配体顶层到底层的零件列表。
> 6. 实现了ask_part_shortname函数，用于获取零件简称。
> 7. 实现了ask_immediate_occs_of_part函数，用于获取父零件中子零件的直接实例。
> 8. 定义了do_it函数，用于获取每个零件在装配体中的实例数，并打印输出。
> 9. 定义了ufusr函数，作为程序的入口函数，用于初始化、执行主要逻辑和终止。
> 10. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 综合来看，这段代码实现了NX装配体的递归遍历，统计了每个零件的实例数，并打印输出每个零件在装配体中的实例分布情况。
>
