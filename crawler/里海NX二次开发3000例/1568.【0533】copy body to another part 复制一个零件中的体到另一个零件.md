### 【0533】copy body to another part 复制一个零件中的体到另一个零件

#### 代码

```cpp
    /*HEAD COPY_BODY_TO_ANOTHER_PART CCC UFUN */  
    /*  The LINKED_BODY created using this method is NOT position independent.  
        See PR 2077670  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_wave.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog是V18版本新增的函数。该函数的功能是用于打印系统日志。 */  
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
    static int ask_all_parts(tag_t **parts)  
    {  
        int  
            err,  
            ii,  
            n = UF_PART_ask_num_parts();  
        if (n > 0)  
        {  
            *parts = UF_allocate_memory(n * sizeof(tag_t), &err);  
            if (UF_CALL(err)) return 0;  
            for (ii = 0; ii < n; ii++)  
            {  
                (*parts)[ii] = UF_PART_ask_nth_part(ii);  
            }  
        }  
        return n;  
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
    static void ask_part_shortname(tag_t part, char *shortname)  
    {  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_PART_ask_part_name(part, partname));  
        UF_CALL(uc4574(partname, 2, shortname));  
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
                case 1:                     /* Back 译:Back to the beginning. */  
                    if (a == 0) return -1;  
                    a = a - 13;  
                    break;  
                case 2:                     /* Cancel 译:Cancel的翻译是取消。 */  
                    return -1;  
                    break;  
                case 18:                    /* More... 译:More...的翻译为：更多... */  
                    if ((a + z) < n_choices)  
                    {  
                        a = a + 13;  
                        break;  
                    }  
                default:                    /* Picked one 译:Picked one */  
                    return a + resp - 5;  
                    break;  
            }  
        } while (TRUE);  
    }  
    static tag_t choose_loaded_part(char *prompt)  
    {  
        int  
            ii,  
            n_parts,  
            which;  
        tag_t  
            the_part,  
            *parts;  
        char  
            **leaf_names;  
        n_parts = ask_all_parts(&parts);  
        UF_CALL(allocate_string_array(n_parts, UF_CFI_MAX_FILE_NAME_LEN, &leaf_names));  
        for (ii = 0; ii < n_parts; ii++) ask_part_shortname(parts[ii], leaf_names[ii]);  
        if ((which = choose_one_item(prompt, leaf_names, n_parts)) == -1)  
            the_part = NULL_TAG;  
        else  
            the_part = parts[which];  
        UF_free(parts);  
        UF_free_string_array(n_parts, leaf_names);  
        return the_part;  
    }  
    /* 里海 */  
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_body(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a body", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_bodies, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            body,  
            feat,  
            copy_to;  
        if (((copy_to = choose_loaded_part("Copy to part")) != NULL_TAG) &&  
            ((body = select_a_body("Body to copy")) != NULL_TAG))  
            UF_CALL(UF_WAVE_create_linked_body(body, NULL_TAG, copy_to, FALSE, &feat));  
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

> 这段NX Open C++代码实现了以下功能：
>
> 1. 错误报告函数：用于在发生UF函数调用错误时输出错误信息。
> 2. 获取当前部件列表：通过UF_PART_ask_num_parts和UF_PART_ask_nth_part函数，可以获取当前部件列表。
> 3. 内存分配函数：用于为字符串数组分配内存。
> 4. 获取部件简称：通过UF_PART_ask_part_name和uc4574函数获取部件的简称。
> 5. 选择列表项：通过uc1603函数实现选择列表项的功能。
> 6. 选择已加载的部件：结合上述函数，实现选择已加载的部件。
> 7. 选择体：通过UF_UI_set_sel_mask、UF_UI_select_with_single_dialog等函数，实现选择体的功能。
> 8. 复制体到另一个部件：通过UF_WAVE_create_linked_body函数，实现将一个体复制到另一个部件的功能。
> 9. 初始化和终止NX：在ufusr函数中初始化和终止NX。
> 10. 卸载请求：在ufusr_ask_unload函数中返回立即卸载请求。
>
> 总的来说，这段代码通过UI交互，实现了将一个部件中的体复制到另一个部件的功能。
>
