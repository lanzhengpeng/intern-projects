### 【2367】report selected tabnote cells 报告所选表格注释的单元格

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_TABNOTE_CELLS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog在V18中新增 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITEnS(n,X) (write_string_array_to_listing_window(#X, X, n))  
    void write_string_array_to_listing_window(char *title, char **strings,  
        int n_strings)  
    {  
        int  
            ii;  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        for (ii = 0; ii < n_strings; ii++)  
        {  
            sprintf(msg, "%s[%d] = %s\n", title, ii, strings[ii]);  
            UF_CALL(UF_UI_write_listing_window(msg));  
        }  
    }  
    static tag_t ask_next_tabular_note(tag_t part, tag_t tnote)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_tabular_note_type,  
            &tnote)) && (tnote != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(tnote, &type, &subtype));  
            if (subtype == UF_tabular_note_subtype) return tnote;  
        }  
        return NULL_TAG;  
    }  
    static int mask_for_tabular_notes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
    //        mask = { UF_tabular_note_type, 0, 0 };  
            mask = { UF_tabular_note_type, UF_tabular_note_subtype, 0 };  
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
    static void do_it(void)  
    {  
        int  
            ii, jj,  
            nm_rows,  
            nm_columns;  
        int  
            subtype,  
            type;  
        tag_t  
            tsection = NULL_TAG,  
            tnote = NULL_TAG,  
            cell = NULL_TAG,  
            row = NULL_TAG,  
            column = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            *cell_text;  
        UF_TABNOT_cell_prefs_t  
            cell_prefs;  
        while ((tsection = select_a_tabular_note("TABNOT")) != NULL_TAG)  
        {  
            WRITE_D(tsection);  
            UF_CALL(UF_OBJ_ask_type_and_subtype(tsection, &type, &subtype));  
            WRITE_D(type); WRITE_D(subtype);  
            if( subtype == UF_tabular_note_section_subtype )  
            {  
                UF_CALL( UF_TABNOT_ask_tabular_note_of_section(tsection, &tnote) );  
                WRITE_D(tnote);  
            }  
            else   
                tnote = tsection;  
            UF_CALL( UF_TABNOT_ask_nm_rows( tnote, &nm_rows) );  
            WRITE_D(nm_rows);  
            UF_CALL( UF_TABNOT_ask_nm_columns( tnote, &nm_columns) );  
            WRITE_D(nm_columns);  
            for( ii = 0; ii < nm_columns; ii++)  
            {  
                for( jj = 0; jj < nm_rows; jj++)  
                {  
                    UF_TABNOT_ask_nth_row( tnote, jj, &row);  
                    UF_TABNOT_ask_nth_column( tnote, ii, &column);  
                    UF_CALL( UF_TABNOT_ask_cell_at_row_col( row, column, &cell) );  
                    WRITE_D(cell);  
                    UF_CALL( UF_TABNOT_ask_cell_text( cell, &cell_text) );  
                    WRITE_S(cell_text);  
                    UF_CALL( UF_TABNOT_ask_cell_prefs( cell, &cell_prefs) );  
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

> 这段代码是用于NX的二次开发，主要功能是选择表格注释，并打印表格注释中的每个单元格的内容。
>
> 代码的主要流程如下：
>
> 1. 包含必要的NX API头文件，并定义了错误报告函数report_error。
> 2. 定义了写整型数和字符串到列表窗口的函数write_integer_to_listing_window和write_string_to_listing_window。
> 3. 定义了选择表格注释的函数select_a_tabular_note，使用UF_UI库的对话框让用户选择表格注释。
> 4. 定义了遍历表格注释中每个单元格，并打印单元格文本的函数do_it。
> 5. 定义了ufusr函数作为程序的入口，初始化NX，调用do_it函数，然后终止NX。
> 6. 定义了卸载函数ufusr_ask_unload，用于立即卸载该代码。
>
> 综上所述，这段代码的主要功能是选择表格注释，遍历每个单元格，并打印单元格内容。通过UF_UI库实现用户交互，通过NX API访问表格注释数据。
>
