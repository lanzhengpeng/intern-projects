### 【1537】list part expressions 列出部件表达式

#### 代码

```cpp
    /*HEAD LIST_PART_EXPRESSIONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能。

注意：只回答译文，不要添加其他内容。 */  
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
    static void ask_part_shortname(tag_t part, char *shortname)  
    {  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_PART_ask_part_name(part, partname));  
        UF_CALL(uc4574(partname, 2, shortname));  
    }  
    static int ask_all_parts(tag_t **parts)  
    {  
        int  
            err,  
            ii,  
            n = UF_PART_ask_num_parts();  
        *parts = UF_allocate_memory(n * sizeof(tag_t), &err);  
        if (UF_CALL(err)) return 0;  
        for (ii = 0; ii < n; ii++)  
        {  
            (*parts)[ii] = UF_PART_ask_nth_part(ii);  
        }  
        return n;  
    }  
    static tag_t ask_tag_of_expression_name(char *exp_name)  
    {  
        tag_t  
            exp_tag;  
        char  
            exp_str[UF_MAX_EXP_LENGTH+1] = { "" },  
            *lhs,  
            *rhs;  
        UF_CALL(UF_MODL_ask_exp(exp_name, exp_str));  
        if (!UF_CALL(UF_MODL_dissect_exp_string(exp_str, &lhs, &rhs, &exp_tag)))  
        {  
            UF_free(lhs);  
            UF_free(rhs);  
            return (exp_tag);  
        }  
        else  
            return NULL_TAG;  
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
    static void report_part_expressions(tag_t part)  
    {  
        int  
            ii,  
            jj,  
            n_exps,  
            n_parts;  
        tag_t  
            *exps,  
            foreign,  
            junk,  
            *parts;  
        char  
            interpart[UF_MAX_EXP_LENGTH+1],  
            *lhs,  
            *rhs,  
            shortname[UF_CFI_MAX_FILE_NAME_SIZE],  
            *string;  
        UF_UI_open_listing_window();  
        ask_part_shortname(part, shortname);  
        n_parts = ask_all_parts(&parts);  
        if (!UF_CALL(UF_MODL_ask_exps_of_part(part, &n_exps, &exps)))  
        {  
            for (ii = 0; ii < n_exps; ii++)  
            {  
            /*  Mark expressions which are controlled by interpart expressions in  
                other parts in the session */  
                UF_CALL(UF_MODL_ask_exp_tag_string(exps[ii], &string));  
                UF_CALL(UF_MODL_dissect_exp_string(string, &lhs, &rhs, &junk));  
                sprintf(interpart, "%s::%s", shortname, lhs);  
                UF_free(lhs);  
                UF_free(rhs);  
                for (jj = 0; jj < n_parts; jj++)  
                {  
                    if (parts[jj] == part) continue;  
                    ensure_part_fully_loaded(parts[jj]);  
                    UF_CALL(UF_ASSEM_set_work_part_quietly(parts[jj], &junk));  
                    foreign = ask_tag_of_expression_name(interpart);  
                    UF_CALL(UF_ASSEM_set_work_part_quietly(part, &junk));  
                    if (foreign != NULL_TAG)  
                    {  
                        UF_UI_write_listing_window("!");  
                        break;  
                    }  
                }  
                UF_UI_write_listing_window(string);  
                UF_UI_write_listing_window("\n");  
                UF_free(string);  
            }  
            if (n_exps > 0) UF_free(exps);  
        }  
        if (n_parts > 0) UF_free(parts);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_ASSEM_ask_work_part();  
        if (part != NULL_TAG) report_part_expressions(part);  
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

> 根据代码内容，这是一段用于NX的二次开发代码，主要功能是列出当前部件的所有表达式，并标注出受其他部件控制的表达式。
>
> 代码的关键功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于输出错误信息。
> 2. 定义了获取部件简称的函数ask_part_shortname。
> 3. 定义了获取所有部件的函数ask_all_parts。
> 4. 定义了根据表达式名获取表达式标签的函数ask_tag_of_expression_name。
> 5. 定义了报告加载状态的函数report_load_status。
> 6. 定义了确保部件完全加载的函数ensure_part_fully_loaded。
> 7. 定义了报告部件表达式的函数report_part_expressions，它会列出当前部件的所有表达式，并标注出受其他部件控制的表达式。
> 8. 定义了主函数do_it，用于执行上述功能。
> 9. 定义了ufusr函数，用于初始化和调用主函数。
> 10. 定义了卸载函数ufusr_ask_unload。
>
> 总的来说，这段代码通过获取当前部件的所有表达式，并与其他部件的表达式进行匹配，实现了列出当前部件的所有表达式，并标注出受其他部件控制的表达式的功能。
>
