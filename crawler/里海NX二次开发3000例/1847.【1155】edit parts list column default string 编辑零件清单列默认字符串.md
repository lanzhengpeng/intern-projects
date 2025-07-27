### 【1155】edit parts list column default string 编辑零件清单列默认字符串

#### 代码

```cpp
    /*HEAD EDIT_PARTS_LIST_COLUMN_DEFAULT_STRING CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_plist.h>  
    #include <uf_tabnot.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中的新增功能，用于打印系统日志。 */  
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
    static int ask_object_layer(tag_t object)  
    {  
        logical  
            is_displayable;  
        UF_OBJ_disp_props_t  
            disp_props;  
        UF_CALL(UF_OBJ_is_displayable(object, &is_displayable));  
        if (is_displayable &&  
            !UF_CALL(UF_OBJ_ask_display_properties(object, &disp_props)))  
                return disp_props.layer;  
        else  
            return -1;  
    }  
    static tag_t ask_parts_list_tag(tag_t part, tag_t plist)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_tabular_note_type,  
            &plist)) && (plist != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(plist, &type, &subtype));  
            if ((subtype == UF_parts_list_subtype) &&  
                (ask_object_layer(plist) > 0)) return plist;  
        }  
        return NULL_TAG;  
    }  
    static logical prompt_for_an_integer(char *prompt, char *item, int *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        int  
            da[1];  
        strncpy(&menu[0][0], item, 15);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n = 1,  
            new_n_rows,  
            n_rows;  
        tag_t  
            cell,  
            column,  
            part = UF_PART_ask_display_part(),  
            plist,  
            row;  
        char  
            default_string[MAX_FIELD_STR_LEN];  
        UF_PLIST_col_prefs_t  
            col_prefs;  
        if ((plist = ask_parts_list_tag(part, NULL_TAG)) == NULL_TAG) return;  
        while (prompt_for_an_integer("Change default string", "Column No.", &n))  
        {  
            if (UF_CALL(UF_TABNOT_ask_nth_column(plist, n-1, &column)))  
                            continue;  
            if (!UF_CALL(UF_PLIST_ask_col_prefs(column, &col_prefs)))  
            {  
                strcpy(default_string, col_prefs.default_string);  
                UF_free(col_prefs.default_string);  
                col_prefs.default_string = default_string;  
                prompt_for_text("Default string", col_prefs.default_string);  
                UF_CALL(UF_PLIST_set_col_prefs(column, &col_prefs));  
            /*  BEGIN work around for PR 1550790 里海译:BEGIN work around for PR 1550790 的翻译如下：

开始处理 PR 1550790 的绕过方案 */  
                UF_CALL(UF_TABNOT_ask_nm_rows(plist, &n_rows));  
                for (ii = 0; ii < n_rows; ii++)  
                {  
                    UF_CALL(UF_TABNOT_ask_nth_row(plist, ii, &row));  
                    UF_CALL(UF_TABNOT_ask_cell_at_row_col(row, column, &cell));  
                    UF_CALL(UF_TABNOT_set_cell_text(cell, default_string));  
                    UF_CALL(UF_TABNOT_ask_nm_rows(plist, &new_n_rows));  
                    if (new_n_rows != n_rows)  
                    {  
                        n_rows = new_n_rows;  
                        ii--;  
                    }  
                }  
            /*  END work around for PR 1550790 里海译:好的，我明白了。这里是翻译：

END work around for PR 1550790的中文翻译是：结束对PR 1550790的变通处理。 */  
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

> 这段代码是一个NX Open API的二次开发示例，其主要功能是修改表格部件的默认字符串。以下是代码的主要内容和功能介绍：
>
> 1. 代码结构：包括头文件、错误处理函数、获取对象图层函数、获取部件列表标签函数、提示输入整数函数、提示输入文本函数、主要逻辑函数和UFusr接口函数。
> 2. 主要功能：通过循环获取当前显示部件的表格注释对象，然后获取表格的列对象，并提示用户输入要修改的列号。用户输入后，会获取该列的默认字符串，并提示用户输入新的默认字符串，然后更新列的默认字符串，并将新字符串应用到所有行单元格中。
> 3. 代码特色：采用面向对象方式，通过封装获取列对象、获取列默认字符串、设置列默认字符串等函数，使代码结构清晰，易于理解和维护。同时，通过错误处理和用户交互提示，提高了代码的健壮性。
> 4. 注意事项：代码中包含了对PR 1550790的绕过方案，用于处理行数变化导致的数组越界问题。
>
> 综上所述，这段代码通过封装和抽象，实现了修改表格列默认字符串的功能，并具有结构清晰、易维护和健壮性等特点。
>
