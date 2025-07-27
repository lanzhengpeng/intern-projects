### 【1882】replace specified text in all notes 在所有注释中替换指定文本

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
    #include <uf_undo.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，我理解如下：

UF_print_syslog 是在 V18 版本中新增的。 */  
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
    static logical prompt_for_two_strings(char *prompt, char *item1,  
        char *item2, char *string1, char *string2)  
    {  
        int  
            ia[2],  
            ip[2] = { 301, 301 },  
            resp;  
        double  
            da[2];  
        char  
            ca[2][31],  
            menu[2][16];  
        strcpy(&menu[0][0], item1);  
        strcpy(&menu[1][0], item2);  
        strcpy(&ca[0][0], string1);  
        strcpy(&ca[1][0], string2);  
        resp = uc1613(prompt, menu, 2, ia, da, ca, ip);  
        if (resp == 3 || resp == 4)  
        {  
            strcpy(string1, &ca[0][0]);  
            strcpy(string2, &ca[1][0]);  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static tag_t ask_next_note(tag_t part, tag_t a_note)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drafting_entity_type,  
            &a_note)) && (a_note != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(a_note, &type, &subtype));  
            if (subtype == UF_draft_note_subtype) return a_note;  
        }  
        return NULL_TAG;  
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
    static void replace_text_in_all_notes(char *old_text, char *new_text)  
    {  
        logical  
            found;  
        int  
            ii, jj,  
            n_text;  
        tag_t  
            a_note = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            note_text[132][133];  
        UF_DRF_draft_aid_text_info_t  
            *text_info;  
        while ((a_note = ask_next_note(part, a_note)) != NULL_TAG)  
        {  
            UF_CALL(UF_DRF_ask_draft_aid_text_info(a_note, &n_text, &text_info));  
            for (ii = 0; ii < n_text; ii++)  
            {  
                if (text_info[ii].text_type != 1) continue;  
                found = FALSE;  
                for (jj = 0; jj < text_info[ii].num_lines; jj++)  
                {  
                    if (replace_characters_in_a_string(  
                        text_info[ii].text[jj].string, old_text, new_text,  
                        &note_text[jj][0], TRUE)) found = TRUE;  
                }  
                if (found) uc5566(a_note, 1, text_info[ii].num_lines, note_text);  
            }  
            UF_CALL(UF_DRF_free_text(n_text, &text_info));  
        }  
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
        char  
            old_text[MAX_LINE_SIZE+1] = { "" },  
            mark_name[1000],  
            new_text[MAX_LINE_SIZE+1] = { "" };  
        while (prompt_for_two_strings("Global text search/replace",  
            "Replace", "With", old_text, new_text))  
        {  
            sprintf(mark_name, "Edit Notes - Replace \"%s\" with \"%s\"",  
                old_text, new_text);  
            set_undo_mark(mark_name);  
            replace_text_in_all_notes(old_text, new_text);  
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

> 这段代码是一个NX二次开发程序，主要功能是全局搜索和替换零件中的注释文本。
>
> 主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于输出错误信息。
> 2. 定义了一个提示输入两个字符串的函数prompt_for_two_strings，用于提示用户输入旧文本和新文本。
> 3. 定义了一个函数ask_next_note，用于遍历零件中的所有注释。
> 4. 定义了一个替换字符串中字符的函数replace_characters_in_a_string，用于将旧文本替换为新文本。
> 5. 定义了一个函数replace_text_in_all_notes，用于遍历零件中的所有注释，并调用replace_characters_in_a_string进行替换。
> 6. 定义了一个设置撤销标记的函数set_undo_mark。
> 7. 主函数do_it中，使用循环调用prompt_for_two_strings获取用户输入，并调用set_undo_mark和replace_text_in_all_notes进行替换操作。
> 8. ufusr函数中，初始化NX环境，调用do_it执行主要功能，最后终止NX环境。
> 9. ufusr_ask_unload函数用于设置程序卸载模式为立即卸载。
>
> 总体来说，该代码实现了在NX零件中全局搜索并替换注释文本的功能，通过提示用户输入旧文本和新文本，循环遍历所有注释进行替换，并提供了撤销功能。
>
