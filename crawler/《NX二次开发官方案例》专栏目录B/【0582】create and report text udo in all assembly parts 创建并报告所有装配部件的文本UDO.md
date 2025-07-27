### 【0582】create and report text udo in all assembly parts 创建并报告所有装配部件的文本UDO

#### 代码

```cpp
    /*HEAD CREATE_AND_REPORT_TEXT_UDO_IN_ALL_ASSEMBLY_PARTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_udobj.h>  
    #include <uf_cfi.h>  
    #include <uf_modl.h>  
    #include <uf_exit.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档描述，UF_print_syslog 是 V18 版本新增的函数。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
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
            the_list,  /* The top down up ordered list 译:从上到下的有序列表 */  
            tmp_list,  /* Used to walk all the assembly parts 译:过去常常步行去所有组件 */  
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
                /*  Move the children to the end of the ordered list 译:移动孩子们到有序列表的末尾。 */  
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
    static void ensure_part_fully_loaded(tag_t part)  
    {  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            error_status;  
        if (!UF_CALL(UF_PART_ask_part_name(part, partname))  
            && ((UF_PART_is_loaded(partname)) != 1))  
        {  
            UF_CALL(UF_PART_open_quiet(partname, &part, &error_status));  
            if (error_status.n_parts > 0) report_load_status(&error_status);  
        }  
    }  
    void create_text_udo(tag_t part)  
    {  
        int  
            now[2] = { -1, -1 };  
        tag_t  
            old_work,  
            udo;  
        char  
            dstr[21],  
            string[MAX_LINE_SIZE+1],  
            *strings[1],  
            tstr[21];  
        UF_UDOBJ_class_t  
            class_id;  
        strings[0] = string;  
        UF_CALL(uc4582(now, 2, dstr, tstr));  
        sprintf(string, "%s %s", dstr, tstr);  
        WRITE("  ");  
        WRITE(string);  
        WRITE("\n");  
        UF_CALL(UF_UDOBJ_ask_class_id_of_name("GTAC_TEXT", &class_id));  
        ensure_part_fully_loaded(part);  
        UF_CALL(UF_ASSEM_set_work_part_quietly(part, &old_work));  
        UF_CALL(UF_UDOBJ_create_udo(class_id, &udo));  
        UF_CALL(UF_UDOBJ_add_strings(udo, 1, strings));  
        UF_CALL(UF_ASSEM_set_work_part_quietly(old_work, &part));  
    }  
    void report_text_udos(tag_t part)  
    {  
        int  
            ii;  
        tag_t  
            udo = NULL_TAG;  
        UF_UDOBJ_class_t  
            class_id;  
        UF_UDOBJ_all_data_t  
            data;  
        UF_CALL(UF_UDOBJ_ask_class_id_of_name("GTAC_TEXT", &class_id));  
        while (!UF_CALL(UF_UDOBJ_cycle_udos_by_class(part, class_id, &udo))  
            && (udo != NULL_TAG))  
        {  
            UF_CALL(UF_UDOBJ_ask_udo_data(udo, &data));  
            for (ii = 0; ii < data.num_strings; ii++)  
            {  
                WRITE("  ");  
                WRITE(data.strings[ii]);  
                WRITE("\n");  
            }  
            UF_CALL(UF_UDOBJ_free_udo_data(&data));  
        }  
    }  
    static logical pick_one_of_two_choices(char *prompt, char *option_one,  
        char *option_two, int *choice)  
    {  
        int  
            resp;  
        char  
            options[2][38];  
        strncpy(&options[0][0], option_one, 37);  
        strncpy(&options[1][0], option_two, 37);  
        options[0][37] = '\0';  
        options[1][37] = '\0';  
        resp = uc1603(prompt, *choice, options, 2);  
        if ((resp > 4) && (resp < 19))  
        {  
            *choice = resp - 4;  /* returns 1 or 2 译:returns 1 or 2 */  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_children,  
            which = 2;  
        tag_t  
            *children,  
            part = UF_PART_ask_display_part();  
        char  
            part_spec[UF_CFI_MAX_PATH_NAME_SIZE];  
        n_children = ask_assembly_parts_top_down(part, &children);  
        while (pick_one_of_two_choices("Text UDOs", "Create", "Report", &which))  
        {  
            for (ii = 0; ii < n_children; ii++)  
            {  
                UF_PART_ask_part_name(children[ii], part_spec);  
                WRITE(part_spec);  
                WRITE(":\n");  
                if (which == 1)  
                    create_text_udo(children[ii]);  
                else  
                    report_text_udos(children[ii]);  
            }  
            WRITE("\n");  
        }  
        if (n_children > 0) UF_free(children);  
    }  
    /* 里海 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    /* 里海 */  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        UF_UDOBJ_class_t  
            class_id;  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_UDOBJ_create_class("GTAC_TEXT", "Text UDO", &class_id));  
        UF_CALL(UF_UDOBJ_set_query_class_id(class_id,  
            UF_UDOBJ_ALLOW_QUERY_CLASS_ID));  
        UF_terminate();  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，其主要功能是在NX部件中创建和报告文本用户定义对象(UDO)。
>
> 代码的主要流程包括：
>
> 1. 定义了错误报告函数report_error，用于打印错误信息。
> 2. 定义了内存分配函数allocate_memory，用于分配内存。
> 3. 定义了创建数组函数make_an_array，用于创建对象数组。
> 4. 定义了报告加载状态函数report_load_status，用于报告部件加载状态。
> 5. 定义了检查对象是否在列表中的函数is_item_on_list。
> 6. 定义了递归查询装配体部件的函数ask_assembly_parts_top_down，用于从顶层部件递归查询所有子部件。
> 7. 定义了确保部件完全加载的函数ensure_part_fully_loaded。
> 8. 定义了创建文本UDO的函数create_text_udo。
> 9. 定义了报告文本UDO的函数report_text_udos。
> 10. 定义了选择对话框函数pick_one_of_two_choices。
> 11. 定义了执行创建或报告的函数do_it。
> 12. 定义了初始化函数ufusr。
> 13. 定义了UDO类创建函数ufsta。
>
> 总的来说，这段代码实现了在NX装配体中创建和报告文本UDO的功能，包含错误处理、内存管理、递归查询、确保加载等辅助功能。
>
