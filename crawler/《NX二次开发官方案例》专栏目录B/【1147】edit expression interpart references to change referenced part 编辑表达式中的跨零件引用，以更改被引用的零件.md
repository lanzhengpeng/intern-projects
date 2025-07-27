### 【1147】edit expression interpart references to change referenced part 编辑表达式中的跨零件引用，以更改被引用的零件

#### 代码

```cpp
    /*HEAD EDIT_EXPRESSION_INTERPART_REFERENCES_TO_CHANGE_REFERENCED_PART CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_undo.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog 是 V18 中新增的。 */  
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
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static int ask_all_parts(tag_t **parts)  
    {  
        int  
            err,  
            ii,  
            n = UF_PART_ask_num_parts();  
        if (n > 0)  
        {  
            *parts = (tag_t *)UF_allocate_memory(n * sizeof(tag_t), &err);  
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
        *where = (char **)UF_allocate_memory(n_elements * sizeof(char *), &resp);  
        for (ii = 0; ii < n_elements; ii++)  
        {  
            (*where)[ii] = (char *)UF_allocate_memory(n_chars*sizeof(char), &resp);  
            if (resp) return resp;  
        }  
        return 0;  
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
                case 1:                     /* Back 里海译:back */  
                    if (a == 0) return -1;  
                    a = a - 13;  
                    break;  
                case 2:                     /* Cancel 里海译:取消 */  
                    return -1;  
                    break;  
                case 18:                    /* More... 里海译:More... */  
                    if ((a + z) < n_choices)  
                    {  
                        a = a + 13;  
                        break;  
                    }  
                default:                    /* Picked one 里海译:选择了一个。 */  
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
        UF_CALL(allocate_string_array(n_parts, UF_CFI_MAX_FILE_NAME_LEN,  
            &leaf_names));  
        for (ii = 0; ii < n_parts; ii++)  
            ask_part_shortname(parts[ii], leaf_names[ii]);  
        if ((which = choose_one_item(prompt, leaf_names, n_parts)) == -1)  
            the_part = NULL_TAG;  
        else  
            the_part = parts[which];  
        UF_free(parts);  
        UF_free_string_array(n_parts, leaf_names);  
        return the_part;  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static logical replace_characters_in_a_string(char *from, char *replace,  
        char *with, char *into, logical verbose)  
    {  
        char  
            *loc,  
            rest[133],  
            *start;  
        strcpy(into, from);  
        if (strstr(from, replace) == NULL) return FALSE;  
        if (verbose) WRITE("Replacing \"");  
        if (verbose) WRITE(from);  
        start = into;  
        while ((loc = strstr(start, replace)) != NULL)  
        {  
            strcpy(rest, loc + strlen(replace));  
            strcpy(loc, with);  
            strcat(into, rest);  
            start = loc + strlen(with);  
        }  
        if (verbose) WRITE("\" with \"");  
        if (verbose) WRITE(into);  
        if (verbose) WRITE("\"\n");  
        return TRUE;  
    }  
    static logical replace_interpart_references(tag_t part, tag_t old_part,  
        tag_t new_part, logical verbose)  
    {  
        logical  
            something_changed = FALSE;  
        int  
            ii,  
            n_exp;  
        tag_t  
            *exps;  
        char  
            *full_string,  
            new_str_q[UF_MAX_EXP_LENGTH],  
            new_str_nq[UF_MAX_EXP_LENGTH],  
            new_full_string[UF_MAX_EXP_LENGTH],  
            npn[UF_CFI_MAX_FILE_NAME_SIZE],  
            old_str_q[UF_MAX_EXP_LENGTH],  
            old_str_nq[UF_MAX_EXP_LENGTH],  
            opn[UF_CFI_MAX_FILE_NAME_SIZE];  
        ask_part_shortname(old_part, opn);  
        ask_part_shortname(new_part, npn);  
        sprintf(old_str_q, "\"%s\"::", opn);  
        sprintf(new_str_q, "\"%s\"::", npn);  
        sprintf(old_str_nq, "%s::", opn);  
        sprintf(new_str_nq, "%s::", npn);  
        UF_CALL(UF_MODL_ask_exps_of_part(part, &n_exp, &exps));  
        if (n_exp > 0)  
        {  
            for (ii = 0; ii < n_exp; ii++)  
            {  
                UF_CALL(UF_MODL_ask_exp_tag_string(exps[ii], &full_string));  
                if (replace_characters_in_a_string(full_string, old_str_q,  
                        new_str_q, new_full_string, verbose) ||  
                    replace_characters_in_a_string(full_string, old_str_nq,  
                        new_str_nq, new_full_string, verbose))  
                {  
                    UF_CALL(UF_MODL_edit_exp(new_full_string));  
                    something_changed = TRUE;  
                }  
                UF_free(full_string);  
            }  
            if (verbose) WRITE_L(something_changed);  
            UF_free(exps);  
        }  
        return something_changed;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            np,  
            op,  
            part = UF_ASSEM_ask_work_part();  
        if (((op = choose_loaded_part("Replace references to")) != NULL_TAG) &&  
            ((np = choose_loaded_part("With new references to")) != NULL_TAG))  
        {  
            set_undo_mark("Replace Interpart References");  
            if (replace_interpart_references(part, op, np, TRUE))  
                UF_CALL(UF_MODL_update());  
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

> 这段代码是一个NX Open C++ API的二次开发示例，主要功能是替换装配体中一个部件的所有引用到另一个部件的引用。
>
> 关键要点包括：
>
> 1. 定义了错误报告函数report_error，用于输出API调用错误。
> 2. 定义了设置撤销标记函数set_undo_mark。
> 3. 定义了写入列表窗口的宏WRITE、WRITE_F、WRITE_L。
> 4. 定义了询问所有部件函数ask_all_parts。
> 5. 定义了为部件分配字符串数组函数allocate_string_array。
> 6. 定义了询问部件简称函数ask_part_shortname。
> 7. 定义了选择一个项目的函数choose_one_item。
> 8. 定义了选择一个已加载部件的函数choose_loaded_part。
> 9. 定义了替换字符串中字符的函数replace_characters_in_a_string。
> 10. 定义了替换部件间引用的函数replace_interpart_references。
> 11. 主函数do_it中，首先选择要替换引用的旧部件，然后选择要替换成的新部件，接着调用replace_interpart_references函数进行替换，最后更新模型。
> 12. NX Open C++ API的入口函数ufusr中，初始化后调用do_it，然后终止。
> 13. 定义了卸载函数ufusr_ask_unload，用于卸载二次开发程序。
>
> 该代码通过NX Open C++ API实现了替换装配体中部件引用的功能，为NX的二次开发提供了实用的参考。
>
