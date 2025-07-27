### 【0940】create tabular note with merged cell header 创建带合并单元格标题的表格注释

#### 代码

```cpp
    /*HEAD CREATE_TABULAR_NOTE_WITH_MERGED_CELL_HEADER CCC UFUN */  
    /*-------------------------------------------  
        uses the new uf_tabnot.h functions that  
        were introduced in NX2 - not suitable  
        for earlier versions  
    -------------------------------------------*/  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_assem.h>  
    #include <uf_tabnot.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    /* qq3123197280 */  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog是在V18版本中新增的。 */  
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
    static void do_it(void)  
    {  
        int  
            nm_sections = 0,  
            nm_columns = 0;  
        tag_t  
            tabnote_section = NULL_TAG,  
            tabnote = NULL_TAG,  
            row = NULL_TAG,  
            column = NULL_TAG,  
            start_cell,  
            end_cell;  
        double  
            origin[3] = { 6., 12., 0. };  
        UF_TABNOT_section_prefs_t   
            def_section_prefs;  
        UF_TABNOT_cell_prefs_t  
            cell_prefs;  
        UF_CALL(UF_TABNOT_ask_default_cell_prefs(&cell_prefs));  
        cell_prefs.nm_fit_methods = 1;  
        cell_prefs.fit_methods[0] = UF_TABNOT_fit_method_overwrite_border;  
        cell_prefs.horiz_just = UF_TABNOT_just_center;  
        UF_CALL( UF_TABNOT_set_default_cell_prefs(&cell_prefs) );  
        UF_CALL( UF_TABNOT_ask_default_section_prefs( &def_section_prefs ));  
        def_section_prefs.header_location = UF_TABNOT_header_location_above;  
        def_section_prefs.max_height = 0.0; /* unlimited 里海译:unlimited的翻译是“无限的”。 */  
        def_section_prefs.overflow_direction = UF_TABNOT_overflow_right;  
        def_section_prefs.overflow_spacing = .1;  
        def_section_prefs.attach_point = UF_TABNOT_attach_point_top_left;  
        def_section_prefs.use_double_width_border = TRUE;  
        def_section_prefs.border_width = 0.0625;  
        UF_CALL( UF_TABNOT_set_default_section_prefs( &def_section_prefs ));  
        UF_CALL( UF_TABNOT_create( &def_section_prefs, origin, &tabnote ));  
        UF_CALL( UF_TABNOT_ask_nm_sections( tabnote, &nm_sections ));  
        UF_CALL( UF_TABNOT_ask_nth_section( tabnote, 0, &tabnote_section ));  
        UF_CALL( UF_TABNOT_create_row( 0.5, &row ));  
        UF_CALL( UF_TABNOT_add_header_row( tabnote, row, 0 ));   
        UF_CALL( UF_TABNOT_ask_nth_column( tabnote, 0, &column ));  
        UF_CALL( UF_TABNOT_ask_cell_at_row_col( row, column, &start_cell ));  
        UF_CALL( UF_TABNOT_set_cell_text( start_cell, "This is the Header Text" ));  
        UF_TABNOT_ask_nm_columns( tabnote, &nm_columns );  
        UF_CALL( UF_TABNOT_ask_nth_column( tabnote, nm_columns-1, &column ));  
        UF_CALL( UF_TABNOT_ask_cell_at_row_col( row, column, &end_cell ));  
        UF_CALL( UF_TABNOT_merge_cells( start_cell, end_cell ));   
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

> 这段代码是用于在NX中创建带合并单元格的表格注释的UFUNC函数。
>
> 主要功能包括：
>
> 1. 定义错误报告函数report_error，用于在调用NX API函数出错时打印错误信息。
> 2. 定义do_it函数，用于创建表格注释并设置其属性：设置默认单元格属性，包括文本对齐方式等。设置默认章节属性，包括标题位置、最大高度等。创建表格注释对象。创建一行作为标题行，并添加到表格注释中。获取标题行的第一个单元格，并设置其文本为"This is the Header Text"。获取标题行的最后一个单元格。合并标题行的第一个单元格和最后一个单元格。
> 3. 设置默认单元格属性，包括文本对齐方式等。
> 4. 设置默认章节属性，包括标题位置、最大高度等。
> 5. 创建表格注释对象。
> 6. 创建一行作为标题行，并添加到表格注释中。
> 7. 获取标题行的第一个单元格，并设置其文本为"This is the Header Text"。
> 8. 获取标题行的最后一个单元格。
> 9. 合并标题行的第一个单元格和最后一个单元格。
> 10. 定义ufusr函数，作为UFUNC函数的入口点，用于初始化NX API，调用do_it函数，然后终止NX API。
> 11. 定义ufusr_ask_unload函数，返回立即卸载，表示该UFUNC函数不需要保持会话状态。
>
> 总的来说，这段代码实现了在NX中创建一个带合并单元格的表格注释的功能，展示了如何使用NX的表格注释API进行二次开发。
>
