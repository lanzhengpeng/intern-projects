### 【1170】edit tabular note header text 编辑表格注释标题文字

#### 代码

```cpp
    /*HEAD EDIT_TABULAR_NOTE_HEADER_TEXT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
    #include <uf_object_types.h>  
    #include <uf_tabnot.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本新增的函数。 */  
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
            jj,  
            n_cols,  
            n_rows;  
        tag_t  
            cell,  
            column,  
            row,  
            tabular_note;  
        char  
            cell_text[133],  
            *old_text,  
            prompt[133];  
        UF_CALL(UF_TABNOT_enable_automatic_update(FALSE));  
        while ((tabular_note = select_a_tabular_note("Edit header")) != NULL_TAG)  
        {  
            UF_CALL(UF_TABNOT_ask_tabular_note_of_section(tabular_note,  
                        &tabular_note));  
            UF_CALL(UF_TABNOT_ask_nm_header_rows(tabular_note, &n_rows));  
            if (n_rows == 0)  
            {  
                uc1601("Tabular note has no header", TRUE);  
                continue;  
            }  
            UF_CALL(UF_TABNOT_ask_nm_columns(tabular_note, &n_cols));  
            for (ii = 0; ii < n_rows; ii++)  
            {  
                UF_CALL(UF_TABNOT_ask_nth_header_row(tabular_note, ii, &row));  
                for (jj = 0; jj < n_cols; jj++)  
                {  
                    UF_CALL(UF_TABNOT_ask_nth_column(tabular_note, jj, &column));  
                    UF_CALL(UF_TABNOT_ask_cell_at_row_col(row, column, &cell));  
                    UF_CALL(UF_TABNOT_ask_cell_text(cell, &old_text));  
                    strncpy(cell_text, old_text, 132);  
                    strcat(cell_text, "");  
                    UF_free(old_text);  
                    sprintf(prompt, "Text for Header Cell (%d, %d)", ii+1, jj+1);  
                    prompt_for_text(prompt, cell_text);  
                    UF_CALL(UF_TABNOT_set_cell_text(cell, cell_text));  
                }  
            }  
            UF_CALL(UF_TABNOT_update(tabular_note));  
        }  
        UF_CALL(UF_TABNOT_enable_automatic_update(TRUE));  
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

> 这段代码是一个NX二次开发示例，其主要功能是编辑表格注释的头部文本。具体功能包括：
>
> 1. 选择表格注释：使用UF_UI接口选择一个表格注释对象。
> 2. 获取表格信息：通过UF_TABNOT接口获取表格的行数和列数。
> 3. 遍历头部单元格：遍历表格的头部单元格，并获取每个单元格的文本。
> 4. 提示用户输入：提示用户输入新的文本以替换原有文本。
> 5. 更新表格：将用户输入的文本更新到表格的头部单元格，并更新表格以保存更改。
>
> 该代码通过UF_CALL宏来捕获并报告错误，同时使用UF_print_syslog打印错误信息。主要逻辑由do_it函数实现，通过用户交互来编辑表格注释的头部文本。
>
