### 【0546】copy reference set members 拷贝参考集成员

#### 代码

```cpp
    /*HEAD COPY_REFERENCE_SET_MEMBERS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_assem.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18中新增的函数，用于打印系统日志。 */  
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
    static tag_t ask_next_ref_set(tag_t part, tag_t refset)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_reference_set_type,  
            &refset)) && (refset != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(refset, &type, &subtype));  
    #ifdef UF_reference_design_model_subtype  
            if ((subtype == UF_reference_design_subtype) ||  
                (subtype == UF_reference_design_model_subtype) ||  
                (subtype == UF_reference_design_lightweight_subtype))  
                return refset;  
    #else  
            if (subtype == UF_reference_design_subtype) return refset;  
    #endif  
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
    static int allocate_string_array(int n_elements, int n_chars, char ***where)  
    {  
        int  
            ii,  
            resp;  
        *where = UF_allocate_memory(n_elements * sizeof(char *), &resp);  
        for (ii = 0; ii < n_elements; ii++)  
        {  
            (*where)[ii] = UF_allocate_memory(n_chars * sizeof(char), &resp);  
            if (resp) return resp;  
        }  
        return 0;  
    }  
    static int choose_one_item(char *prompt, char **choices, int n_choices)  
    {  
        int  
            a = 0,  
            ii,  
            resp,  
            z;  
        char  
            options[14][38];  
        do  
        {  
            z = (n_choices - a < 14) ? n_choices - a : 14;  
            for (ii = 0; ii < z; ii++)  
            {  
                strncpy(&options[ii][0], choices[a+ii], 37);  
                options[ii][37] = '\0';  
            }  
            if ((z == 14) && ((a + z) < n_choices))  
                strcpy(&options[13][0], "More...");  
            switch (resp = uc1603(prompt, 0, options, z))  
            {  
                case 1:                     /* Back 译:Back */  
                    if (a == 0) return -1;  
                    a = a - 13;  
                    break;  
                case 2:                     /* Cancel 译:Cancel的翻译是“取消”。 */  
                    return -1;  
                    break;  
                case 18:                    /* More... 译:翻译More... */  
                    if ((a + z) < n_choices)  
                    {  
                        a = a + 13;  
                        break;  
                    }  
                default:                    /* Picked one 译:"挑选一个" */  
                    return a + resp - 5;  
                    break;  
            }  
        } while (TRUE);  
    }  
    static tag_t choose_a_reference_set(char *prompt)  
    {  
        int  
            ii,  
            n_refsets,  
            which;  
        tag_t  
            part = UF_ASSEM_ask_work_part(),  
            the_refset,  
            *refsets;  
        char  
            **refset_names;  
        n_refsets = ask_all_reference_sets(part, &refsets);  
        UF_CALL(allocate_string_array(n_refsets, MAX_ENTITY_NAME_SIZE+1,  
            &refset_names));  
        for (ii = 0; ii < n_refsets; ii++)  
            UF_CALL(UF_OBJ_ask_name(refsets[ii], refset_names[ii]));  
        if ((which = choose_one_item(prompt, refset_names, n_refsets)) == -1)  
            the_refset = NULL_TAG;  
        else  
            the_refset = refsets[which];  
        UF_free(refsets);  
        UF_free_string_array(n_refsets, refset_names);  
        return the_refset;  
    }  
    #include <uf_undo.h>  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n,  
            subtype,  
            type;  
        tag_t  
            from,  
            *members,  
            root = UF_ASSEM_ask_root_part_occ(UF_ASSEM_ask_work_part()),  
            to;  
        while (((from = choose_a_reference_set("Copy members from")) != NULL_TAG)  
            && ((to = choose_a_reference_set("Copy members to")) != NULL_TAG))  
        {  
            set_undo_mark("Copy Reference Set Members");  
            UF_CALL(UF_ASSEM_ask_ref_set_members(from, &n, &members));  
            for (ii = 0; ii < n; ii++)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(members[ii], &type, &subtype));  
                if (type == UF_occ_instance_type)  
                    members[ii] = UF_ASSEM_ask_part_occ_of_inst(root, members[ii]);  
            }  
            if (n > 0)  
            {  
                UF_CALL(UF_ASSEM_add_ref_set_members(to, n, members));  
                UF_free(members);  
            }  
        }  
    }  
    /* 里海 */  
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

> 这段代码是NX的二次开发代码，其主要功能是在NX中复制参考集中的成员。代码的主要逻辑包括：
>
> 1. 错误报告函数：用于在函数调用出错时报告错误信息。
> 2. 参考集遍历函数：用于遍历部件中的所有参考集，并返回下一个参考集的tag。
> 3. 内存分配函数：用于分配内存空间。
> 4. 列表转数组函数：用于将UF列表转换为数组。
> 5. 获取所有参考集函数：用于获取部件中所有的参考集。
> 6. 字符串数组分配函数：用于为字符串数组分配内存。
> 7. 选择一个条目函数：用于在多个选项中选择一个。
> 8. 选择参考集函数：用于让用户选择一个参考集。
> 9. 设置撤销标记函数：用于设置撤销标记。
> 10. 执行复制函数：是主函数，用于执行复制参考集成员的操作。包括获取源参考集和目标参考集，设置撤销标记，获取源参考集的成员，转换成员类型，将成员添加到目标参考集，并释放内存。
> 11. ufusr函数：是NX调用的主函数，用于初始化、执行复制操作，并终止。
> 12. 卸载函数：用于处理卸载操作。
>
> 通过以上函数，代码实现了NX中复制参考集成员的功能。
>
