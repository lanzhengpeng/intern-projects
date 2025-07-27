### 【3068】write part expressions to part attributes 将零件表达式写入零件属性

#### 代码

```cpp
    /*HEAD WRITE_PART_EXPRESSIONS_TO_PART_ATTRIBUTES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_attr.h>  
    #include <uf_part.h>  
    #include <uf_undo.h>  
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
    static void write_part_attribute(char *title, char *val)  
    {  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            str[UF_ATTR_MAX_STRING_LEN+1];  
        UF_ATTR_value_t  
            value;  
        value.type = UF_ATTR_string;  
    /*  set the pointer to point to the declared character space 里海译:将指针指向声明的字符空间 */  
        value.value.string = str;  
        strcpy(value.value.string, val);  
        UF_CALL(UF_ATTR_assign(part, title, value));  
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
    static void report_expressions_and_write_to_attributes(tag_t part)  
    {  
        int  
            ii,  
            jj,  
            n_exps,  
            n_parts;  
        double  
            val;  
        tag_t  
            *exps,  
            foreign,  
            junk,  
            *parts;  
        char  
            interpart[UF_MAX_EXP_LENGTH+1],  
            *lhs,  
            *rhs,  
            valstr[UF_MAX_EXP_LENGTH+1],  
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
                UF_CALL(UF_MODL_eval_exp(lhs, &val));  
                sprintf(interpart, "%s::%s", shortname, lhs);  
                sprintf(valstr,"%f",val);  
                write_part_attribute(lhs, valstr);  
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
    static void report_part_attributes(tag_t part)  
    {  
        int  
            ii,  
            n_attrs;  
        UF_ATTR_part_attr_t  
            *attrs;  
        char  
            msg[133],  
            pspec[MAX_FSPEC_SIZE+1];  
        UF_PART_ask_part_name(part, pspec);  
        UF_CALL(UF_ATTR_ask_part_attrs(part, &n_attrs, &attrs));  
        sprintf(msg,"%s has %d part attributes\n", pspec, n_attrs);  
        if (!UF_UI_open_listing_window())  
        {  
            UF_UI_write_listing_window(msg);  
        }  
        for (ii = 0; ii < n_attrs; ii++)  
        {  
            sprintf(msg, "  %d.  %s = %s\n", ii+1, attrs[ii].title,  
                attrs[ii].string_value);  
            UF_UI_write_listing_window(msg);  
        }  
        if (n_attrs > 0) UF_free(attrs);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_ASSEM_ask_work_part();  
        UF_UNDO_mark_id_t  
            mark_id;  
        if (part != NULL_TAG)  
        {  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window("Expressions: \n");  
            }  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, "Write Expressions to Attributes", &mark_id));  
            report_expressions_and_write_to_attributes(part);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window("\nPart Attributes: \n");  
            }  
            report_part_attributes(part);  
        }  
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

> 这段代码是一个NX二次开发的应用程序，其主要功能是将零件的表达式赋值给零件的属性。具体来说：
>
> 1. 该代码定义了一个报告错误函数report_error，用于在发生UF调用错误时打印错误信息。
> 2. 定义了一个write_part_attribute函数，用于将字符串值赋给零件的属性。
> 3. ask_part_shortname函数用于获取零件的简称。
> 4. ask_all_parts函数用于获取当前会话中的所有零件。
> 5. ask_tag_of_expression_name函数用于根据表达式名称查询表达式标签。
> 6. report_load_status函数用于报告零件加载状态。
> 7. ensure_part_fully_loaded函数用于确保零件被完全加载。
> 8. report_expressions_and_write_to_attributes函数用于遍历零件的表达式，并将表达式值赋给零件属性。
> 9. report_part_attributes函数用于报告零件的属性。
> 10. do_it函数是主函数，用于获取当前工作零件，设置撤销标记，遍历零件表达式并赋值给属性，最后报告零件属性。
> 11. ufusr函数是NX调用的入口函数，用于初始化、执行do_it函数，然后终止。
> 12. ufusr_ask_unload函数用于卸载应用程序。
>
> 总体来说，该应用程序实现了将零件的表达式赋值给零件的属性的功能。
>
