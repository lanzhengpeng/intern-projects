### 【0071】add or edit tabular note rows 添加或编辑表格注释行

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
    #include <uf_object_types.h>  
    #include <uf_tabnot.h>  
    #include <uf_obj.h>  
    #include <uf_plist.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    /* 里海 */  
    static int mask_for_tabular_notes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_tabular_note_type, UF_tabular_note_section_subtype, 0 };  
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
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_tabular_notes, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
            *choice = resp - 4;  /* returns 1 or 2 译:返回1或2 */  
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
    static void add_or_edit_tabular_note_rows(void)  
    {  
        int  
            ii,  
            n,  
            n_cols,  
            n_rows,  
            subtype,  
            type,  
            which = 1;  
        tag_t  
            cell,  
            column,  
            last_row,  
            new_row = NULL_TAG,  // must be initialized as of NX8 - See PR 6800195  
            tabular_note;  
        double  
            height;  
        char  
            cell_text[133],  
            *old_text,  
            prompt[133];  
        while ((tabular_note = select_a_tabular_note("Add/Edit row")) != NULL_TAG)  
        {  
            UF_CALL(UF_TABNOT_ask_tabular_note_of_section(tabular_note,  
                        &tabular_note));  
            UF_CALL(UF_TABNOT_ask_nm_rows(tabular_note, &n_rows));  
            UF_CALL(UF_TABNOT_ask_nm_columns(tabular_note, &n_cols));  
            UF_CALL(UF_TABNOT_ask_nth_row(tabular_note, n_rows-1, &last_row));  
            UF_CALL(UF_TABNOT_ask_row_height(last_row, &height));  
            n = n_rows + 1;  
            if (!prompt_for_an_integer("Add/Edit row", "row number", &n)) continue;  
            n = n - 1;  
            if (n >= n_rows)  
            {  
                n = UF_TABNOT_APPEND;  
                which = 1;  
            }  
            else  
            {  
                if (!pick_one_of_two_choices("Which?", "Add row", "Edit row",  
                    &which)) continue;  
            }  
            if (which == 1)  
            {  
                UF_CALL(UF_TABNOT_enable_automatic_update(TRUE));  
                UF_CALL(UF_OBJ_ask_type_and_subtype(tabular_note, &type,  
                    &subtype));  
                if (subtype == UF_parts_list_subtype)  
                    UF_CALL(UF_PLIST_create_manual_row(height, &new_row));  
                else  
                    UF_CALL(UF_TABNOT_create_row(height, &new_row));  
                 UF_CALL(UF_TABNOT_add_row(tabular_note, new_row, n));  
            }  
            else  
            {  
                UF_CALL(UF_TABNOT_ask_nth_row(tabular_note, n, &new_row));  
            }  
            if (n == UF_TABNOT_APPEND) n = n_rows;  
            UF_CALL(UF_TABNOT_enable_automatic_update(FALSE));  
            for (ii = 0; ii < n_cols; ii++)  
            {  
                UF_CALL(UF_TABNOT_ask_nth_column(tabular_note, ii, &column));  
                UF_CALL(UF_TABNOT_ask_cell_at_row_col(new_row, column, &cell));  
                UF_CALL(UF_TABNOT_ask_cell_text(cell, &old_text));  
                UF_CALL(UF_DISP_set_highlight(cell, TRUE));  // see PR 6823176  
                strncpy(cell_text, old_text, 132);  
                strcat(cell_text, "");  
                UF_free(old_text);  
                sprintf(prompt, "Text for Cell (%d, %d)", n+1, ii+1);  
                prompt_for_text(prompt, cell_text);  
                UF_CALL(UF_TABNOT_set_cell_text(cell, cell_text));  
                UF_CALL(UF_DISP_set_highlight(cell, FALSE));  // see PR 6823176  
            }  
            UF_CALL(UF_TABNOT_update(tabular_note));  
        }  
    }  
    /* 里海 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        add_or_edit_tabular_note_rows();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX的二次开发示例，主要功能是添加和编辑表格注释的行。以下是代码的主要功能介绍：
>
> 1. 初始化和错误报告：代码首先初始化NX环境，并定义了一个错误报告函数report_error，用于在出错时输出错误信息。
> 2. 表格注释行选择：定义了一个函数select_a_tabular_note，用于提示用户选择一个表格注释对象。
> 3. 提示输入：定义了几个函数，用于提示用户输入数字、选择、或文本。
> 4. 添加/编辑行：函数add_or_edit_tabular_note_rows是主要逻辑。首先选择一个表格注释对象，然后获取其行数和列数。接着提示用户输入要添加或编辑的行号。如果是添加行，则调用UF_TABNOT_create_row创建新行并添加到表格注释中；如果是编辑行，则获取该行。然后遍历所有列，提示用户输入每个单元格的文本，并更新单元格文本。最后调用UF_TABNOT_update更新表格注释。
> 5. 主函数：函数ufusr是程序的入口，调用add_or_edit_tabular_note_rows执行主要逻辑。
> 6. 卸载函数：函数ufusr_ask_unload用于在卸载时立即清理环境。
>
> 总体来说，这段代码实现了基本的表格注释行添加和编辑功能，并包含了错误处理、用户输入提示等辅助功能。
>
