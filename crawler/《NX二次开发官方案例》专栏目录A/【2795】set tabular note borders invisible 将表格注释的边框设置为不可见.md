### 【2795】set tabular note borders invisible 将表格注释的边框设置为不可见

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
    static void do_it(void)  
    {  
        int  
            cfw[3],  
            ii, jj,  
            n_cols,  
            n_headers,  
            n_rows;  
        tag_t  
            cell,  
            col,  
            row,  
            section,  
            tabular_note;  
        UF_TABNOT_cell_prefs_t  
            cell_prefs;  
        UF_CALL(UF_TABNOT_enable_automatic_update(FALSE));  
        while ((section = select_a_tabular_note("Remove table grid lines")) != NULL_TAG)  
        {  
            UF_CALL(UF_TABNOT_ask_tabular_note_of_section(section, &tabular_note));  
            UF_CALL(UF_TABNOT_ask_nm_columns(tabular_note, &n_cols));  
            UF_CALL(UF_TABNOT_ask_nm_header_rows(tabular_note, &n_headers));  
            UF_CALL(UF_TABNOT_ask_nm_rows(tabular_note, &n_rows));  
            for (ii = 0; ii < n_headers; ii++)  
            {  
                UF_CALL(UF_TABNOT_ask_nth_header_row(tabular_note, ii, &row));  
                UF_CALL(UF_TABNOT_ask_row_head_cfw(row, cfw));  
                cfw[1] = UF_OBJ_FONT_INVISIBLE;  
                UF_CALL(UF_TABNOT_set_row_head_cfw(row, cfw));  
            }  
            for (ii = 0; ii < n_rows; ii++)  
            {  
                UF_CALL(UF_TABNOT_ask_nth_row(tabular_note, ii, &row));  
                UF_CALL(UF_TABNOT_ask_row_head_cfw(row, cfw));  
                cfw[1] = UF_OBJ_FONT_INVISIBLE;  
                UF_CALL(UF_TABNOT_set_row_head_cfw(row, cfw));  
            }  
            for (jj = 0; jj < n_cols; jj++)  
            {  
                UF_CALL(UF_TABNOT_ask_nth_column(tabular_note, jj, &col));  
                UF_CALL(UF_TABNOT_ask_column_head_cfw(col, section, cfw));  
                cfw[1] = UF_OBJ_FONT_INVISIBLE;  
                UF_CALL(UF_TABNOT_set_column_head_cfw(col, section, cfw));  
                for (ii = 0; ii < n_headers; ii++)  
                {  
                    UF_CALL(UF_TABNOT_ask_nth_header_row(tabular_note, ii, &row));  
                    UF_CALL(UF_TABNOT_ask_cell_at_row_col(row, col, &cell));  
                    UF_CALL(UF_TABNOT_ask_cell_prefs(cell, &cell_prefs));  
                    cell_prefs.bottom_line_cfw[1] = UF_OBJ_FONT_INVISIBLE;  
                    cell_prefs.right_line_cfw[1] = UF_OBJ_FONT_INVISIBLE;  
                    UF_CALL(UF_TABNOT_set_cell_prefs(cell, &cell_prefs));  
                }  
                for (ii = 0; ii < n_rows; ii++)  
                {  
                    UF_CALL(UF_TABNOT_ask_nth_row(tabular_note, ii, &row));  
                    UF_CALL(UF_TABNOT_ask_cell_at_row_col(row, col, &cell));  
                    UF_CALL(UF_TABNOT_ask_cell_prefs(cell, &cell_prefs));  
                    cell_prefs.bottom_line_cfw[1] = UF_OBJ_FONT_INVISIBLE;  
                    cell_prefs.right_line_cfw[1] = UF_OBJ_FONT_INVISIBLE;  
                    UF_CALL(UF_TABNOT_set_cell_prefs(cell, &cell_prefs));  
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

> 这段代码主要用于在NX中隐藏表格注释的网格线。以下是代码的主要功能：
>
> 1. 功能函数：定义了ECHO和report_error函数，用于打印信息和报告错误。
> 2. 选择表格注释：使用mask_for_tabular_notes函数设置选择过滤器，以选择表格注释对象。
> 3. 隐藏网格线：使用select_a_tabular_note函数选择一个表格注释对象，并调用一系列函数来获取表格注释的列、行、单元格等对象，然后设置相关属性为不可见，从而实现隐藏网格线的效果。
> 4. 自动更新：在操作过程中，关闭了表格注释的自动更新，以避免频繁更新导致的效率问题。
> 5. 主体函数：do_it函数循环选择表格注释，并隐藏其网格线，直到用户取消选择。
> 6. NX初始化：ufusr函数首先初始化NX，然后调用do_it函数执行主体逻辑，最后终止NX。
> 7. 卸载询问：ufusr_ask_unload函数用于询问是否立即卸载当前加载项。
>
> 整体来看，这段代码实现了隐藏NX中表格注释网格线的主要功能。
>
