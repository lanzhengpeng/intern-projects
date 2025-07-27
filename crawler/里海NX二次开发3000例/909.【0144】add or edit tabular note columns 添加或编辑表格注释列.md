### 【0144】add or edit tabular note columns 添加或编辑表格注释列

#### 代码

```cpp
    /*HEAD ADD_OR_EDIT_TABULAR_NOTE_COLUMNS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
    #include <uf_object_types.h>  
    #include <uf_tabnot.h>  
    #include <uf_obj.h>  
    #include <uf_plist.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog 是 V18 中新增的功能。 */  
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
            *choice = resp - 4;  /* returns 1 or 2 译:翻译returns 1 or 2。 */  
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
            n,  
            n_cols,  
            n_headers,  
            n_rows,  
            which = 1;  
        tag_t  
            cell,  
            last_col,  
            new_col,  
            row,  
            section,  
            tabular_note;  
        double  
            width;  
        char  
            cell_text[133],  
            *old_text,  
            prompt[133];  
        while ((section = select_a_tabular_note("Add/Edit Column")) != NULL_TAG)  
        {  
            UF_CALL(UF_TABNOT_ask_tabular_note_of_section(section, &tabular_note));  
            UF_CALL(UF_TABNOT_ask_nm_rows(tabular_note, &n_rows));  
            UF_CALL(UF_TABNOT_ask_nm_columns(tabular_note, &n_cols));  
            UF_CALL(UF_TABNOT_ask_nth_column(tabular_note, n_cols-1, &last_col));  
            UF_CALL(UF_TABNOT_ask_column_width(last_col, &width));  
            n = n_cols + 1;  
            if (!prompt_for_an_integer("Add/Edit column", "column number", &n))  
                continue;  
            n = n - 1;  
            if (n >= n_cols)  
            {  
                n = UF_TABNOT_APPEND;  
                which = 1;  
            }  
            else  
            {  
                if (!pick_one_of_two_choices("Which?", "Add column", "Edit column",  
                    &which)) continue;  
            }  
            if (which == 1)  
            {  
                UF_CALL(UF_TABNOT_enable_automatic_update(TRUE));  
                UF_CALL(UF_TABNOT_create_column(width, &new_col));  
                UF_CALL(UF_TABNOT_add_column(tabular_note, new_col, n));  
            }  
            else  
            {  
                UF_CALL(UF_TABNOT_ask_nth_column(tabular_note, n, &new_col));  
            }  
            if (n == UF_TABNOT_APPEND) n = n_cols;  
            UF_CALL(UF_TABNOT_enable_automatic_update(FALSE));  
            UF_CALL(UF_TABNOT_ask_nm_header_rows(tabular_note, &n_headers));  
            for (ii = 0; ii < n_headers; ii++)  
            {  
                UF_CALL(UF_TABNOT_ask_nth_header_row(tabular_note, ii, &row));  
                UF_CALL(UF_TABNOT_ask_cell_at_row_col(row, new_col, &cell));  
                UF_CALL(UF_TABNOT_ask_cell_text(cell, &old_text));  
                strncpy(cell_text, old_text, 132);  
                strcat(cell_text, "");  
                UF_free(old_text);  
                sprintf(prompt, "Text for Header Cell (%d, %d)", ii+1, n+1);  
                prompt_for_text(prompt, cell_text);  
                UF_CALL(UF_TABNOT_set_cell_text(cell, cell_text));  
            }  
            for (ii = 0; ii < n_rows; ii++)  
            {  
                UF_CALL(UF_TABNOT_ask_nth_row(tabular_note, ii, &row));  
                UF_CALL(UF_TABNOT_ask_cell_at_row_col(row, new_col, &cell));  
                UF_CALL(UF_TABNOT_ask_cell_text(cell, &old_text));  
                strncpy(cell_text, old_text, 132);  
                strcat(cell_text, "");  
                UF_free(old_text);  
                sprintf(prompt, "Text for Cell (%d, %d)", ii+1, n+1);  
                prompt_for_text(prompt, cell_text);  
                UF_CALL(UF_TABNOT_set_cell_text(cell, cell_text));  
            }  
            UF_CALL(UF_TABNOT_update(tabular_note));  
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

> 这段代码是一个NX的二次开发示例，主要实现了在表格注释中添加或编辑列的功能。代码的主要功能包括：
>
> 1. 选择表格注释：用户可以交互式选择一个表格注释对象。
> 2. 添加或编辑列：用户可以指定要添加或编辑的列号，如果是添加操作，会自动插入新列；如果是编辑操作，会修改指定列的文本。
> 3. 输入文本：用户可以输入列标题和每个单元格的文本，并将其更新到表格注释对象中。
> 4. 错误处理：使用UF_CALL宏来检查函数调用是否成功，并输出错误信息。
> 5. 自动更新：在添加新列时，使用UF_TABNOT_enable_automatic_update函数来启用自动更新，以简化添加操作。
> 6. 内存释放：在处理单元格文本时，释放了使用UF_free分配的内存。
> 7. 交互式输入：使用多个交互式对话框来获取用户输入，如列号、添加/编辑选择、列标题和单元格文本。
> 8. 更新表格注释：最后调用UF_TABNOT_update函数来更新表格注释对象。
>
> 整体而言，该代码实现了表格注释的列管理功能，用户可以通过简单的交互操作来维护表格注释的内容，同时提供了完善的错误处理机制。
>
