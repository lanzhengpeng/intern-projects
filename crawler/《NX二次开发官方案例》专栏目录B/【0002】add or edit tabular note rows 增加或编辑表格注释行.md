### 【0002】add or edit tabular note rows 增加或编辑表格注释行

#### 代码

```cpp
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
            *choice = resp - 4;  /* returns 1 or 2 译:根据文中内容，returns 1 or 2的翻译为“返回1或2”。 */  
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
            which = 1;  
        tag_t  
            cell,  
            column,  
            last_row,  
            new_row,  
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
                strncpy(cell_text, old_text, 132);  
                strcat(cell_text, "");  
                UF_free(old_text);  
                sprintf(prompt, "Text for Cell (%d, %d)", n+1, ii+1);  
                prompt_for_text(prompt, cell_text);  
                UF_CALL(UF_TABNOT_set_cell_text(cell, cell_text));  
            }  
            UF_CALL(UF_TABNOT_update(tabular_note));  
        }  
    }

```

#### 代码解析

> 这是NX二次开发代码，其主要功能是添加或编辑表格注释中的行。以下是代码的主要逻辑：
>
> 1. 通过对话框提示用户选择一个表格注释对象。
> 2. 查询该表格注释的行数和列数。
> 3. 根据用户输入的行号，判断是添加新行还是编辑已有行。
> 4. 如果是添加新行，则创建新行对象，并添加到表格注释中。
> 5. 如果是编辑已有行，则查询该行的对象。
> 6. 遍历该行的所有单元格，提示用户输入每个单元格的文本，并更新单元格文本。
> 7. 更新表格注释对象。
> 8. 重复上述过程，直到用户不再选择表格注释对象。
> 9. 在整个过程中，使用了大量NX的表格注释相关的API，如UF_TABNOT_ask_tabular_note_of_section、UF_TABNOT_ask_cell_text等。
> 10. 同时也使用了NX的UI相关的API，如UF_UI_select_with_single_dialog，用于与用户交互。
>
> 总之，这段代码实现了添加或编辑表格注释中的行的功能，通过用户交互获取所需信息，然后调用NX的API完成操作。
>
