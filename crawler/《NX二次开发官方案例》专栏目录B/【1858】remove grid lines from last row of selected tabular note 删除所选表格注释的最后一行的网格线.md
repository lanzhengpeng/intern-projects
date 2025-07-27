### 【1858】remove grid lines from last row of selected tabular note 删除所选表格注释的最后一行的网格线

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
            jj,  
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
        while ((section = select_a_tabular_note("Remove grid lines from last row only"))   
            != NULL_TAG)  
        {  
            UF_CALL(UF_TABNOT_ask_tabular_note_of_section(section, &tabular_note));  
            UF_CALL(UF_TABNOT_ask_nm_columns(tabular_note, &n_cols));  
            UF_CALL(UF_TABNOT_ask_nm_header_rows(tabular_note, &n_headers));  
            UF_CALL(UF_TABNOT_ask_nm_rows(tabular_note, &n_rows));  
            UF_CALL(UF_TABNOT_ask_nth_row(tabular_note, n_rows-1, &row));  
            UF_CALL(UF_TABNOT_ask_row_head_cfw(row, cfw));  
            cfw[1] = UF_OBJ_FONT_INVISIBLE;  
            UF_CALL(UF_TABNOT_set_row_head_cfw(row, cfw));  
            for (jj = 0; jj < n_cols; jj++)  
            {  
                UF_CALL(UF_TABNOT_ask_nth_column(tabular_note, jj, &col));  
                UF_CALL(UF_TABNOT_ask_cell_at_row_col(row, col, &cell));  
                UF_CALL(UF_TABNOT_ask_cell_prefs(cell, &cell_prefs));  
                cell_prefs.bottom_line_cfw[1] = UF_OBJ_FONT_INVISIBLE;  
                cell_prefs.right_line_cfw[1] = UF_OBJ_FONT_INVISIBLE;  
                UF_CALL(UF_TABNOT_set_cell_prefs(cell, &cell_prefs));  
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

> 这是NX的二次开发代码，主要实现了以下功能：
>
> 1. 错误报告：定义了一个ECHO宏，用于打印错误信息到日志窗口和系统日志，方便调试。
> 2. 表格注释选择：实现了mask_for_tabular_notes函数，用于设置选择器，只选择表格注释对象。实现了select_a_tabular_note函数，用于弹出一个选择对话框，让用户选择一个表格注释对象。
> 3. 移除表格线：do_it函数实现了主要功能。首先禁用表格注释的自动更新，然后循环选择每个表格注释对象。对于每个表格注释，获取其列数、标题行数和总行数，然后获取最后一行。设置最后一行的表头线为不可见，然后遍历所有列，设置最后一行每个单元格的底线和右线为不可见。最后更新表格注释。
> 4. 自动更新：在do_it函数的开始和结束处，分别禁用和启用表格注释的自动更新，避免更新过程中频繁的刷新。
> 5. 初始化和终止：在ufusr函数中，初始化NX环境，调用do_it执行主体功能，然后终止NX环境。
> 6. 卸载请求：ufusr_ask_unload函数返回立即卸载，表示二次开发应用程序可以立即被卸载。
>
> 总的来说，该代码通过NX提供的API实现了移除表格注释最后一行的表格线的效果，主要使用表格注释相关的函数来实现。
>
