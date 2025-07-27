### 【1165】edit selected tabular note section maximum height 修改选定表格注释部分的最大高度

#### 代码

```cpp
    /*HEAD EDIT_SELECTED_TABULAR_NOTE_SECTION_MAXIMUM_HEIGHT CCC UFUN */  
    /*  
        When modifing the max.  height of a single section - the position  
        of other sections are not re-evaluated - See PR 6048511  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_object_types.h>  
    #include <uf_tabnot.h>  
    #include <uf_plist.h>  
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
    /* qq3123197280 */  
    static int mask_for_tabular_notes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_tabular_note_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_tabular_note(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a tabular note", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_tabular_notes, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strncpy(&menu[0][0], item, 16);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
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
            *choice = resp - 4;  /* returns 1 or 2 里海译:翻译为：

返回1或2。 */  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void do_it(void)  
    {  
        int  
            n_sections,  
            which;  
        tag_t  
            section,  
            tab_note;  
        double  
            height;  
        UF_TABNOT_section_prefs_t  
            section_prefs;  
        UF_CALL(UF_TABNOT_enable_automatic_update(TRUE));  
        while ((section = select_a_tabular_note("Edit maximum height")) != NULL_TAG)  
        {  
            UF_CALL(UF_TABNOT_ask_section_prefs(section, &section_prefs));  
            if (prompt_for_a_number("Change Section Style", "Maximum Height",  
                &section_prefs.max_height))  
            {  
                UF_CALL(UF_TABNOT_ask_tabular_note_of_section(section, &tab_note));  
                UF_CALL(UF_TABNOT_ask_nm_sections(tab_note, &n_sections));  
                if ((n_sections > 1) &&  
                    pick_one_of_two_choices("Apply to", "This Section",  
                        "All Sections", &which) && (which == 2))  
                {  
                    set_undo_mark("Edit Tabular Note Maximum Height");  
                    UF_CALL(UF_TABNOT_ask_nth_section(tab_note, 0, &section));  
                    height = section_prefs.max_height;  
                    section_prefs.max_height = 0;  
                    UF_CALL(UF_TABNOT_set_section_prefs(section, &section_prefs));  
                    section_prefs.max_height = height;  
                    UF_CALL(UF_TABNOT_set_section_prefs(section, &section_prefs));  
                }  
                else  
                {  
                    set_undo_mark("Edit Tabular Note Section Maximum Height");  
                    UF_CALL(UF_TABNOT_set_section_prefs(section, &section_prefs));  
                }  
            }  
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

> 这段NX二次开发代码的主要功能是修改表格注释的每个区域的最大高度，具体步骤如下：
>
> 1. 初始化NX环境，启用表格注释的自动更新功能。
> 2. 循环提示用户选择一个表格注释，直到用户选择“完成”。
> 3. 对用户选择的每个表格注释，获取其所有区域的最大高度等设置。
> 4. 提示用户修改区域的最大高度，用户可以输入新值。
> 5. 提示用户选择是否只修改当前区域的最大高度，还是修改所有区域的最大高度。
> 6. 如果用户选择修改所有区域，则先设置第一个区域的最大高度为0，然后逐个修改所有区域的最大高度。
> 7. 如果用户选择只修改当前区域，则直接修改当前区域的最大高度。
> 8. 在修改前后设置撤销标记，以便用户可以撤销操作。
> 9. 重复步骤2-8，直到用户选择“完成”。
> 10. 释放NX环境，结束程序。
>
> 代码利用了NX的表格注释API，实现了交互式的修改表格注释每个区域的最大高度，考虑了用户的选择，并提供了撤销功能。
>
