### 【0935】create tabnot using title cell 创建表格注释并使用标题单元格

#### 代码

```cpp
    /*HEAD CREATE_TABNOT_USING_TITLE_CELL CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_draw.h>  
    #include <uf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的功能。 */  
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
        tag_t  
            new_tab_note;  
        char  
            *fonts[] = { "blockfont"} ;  
        UF_DRAW_tabnot_params_t  
            parms;  
        UF_DRAW_tabnot_cell_params_t  
            cell_params;  
        UF_CALL( UF_DRAW_ask_tabular_note_defaults(&parms));  
        parms.position[0] = 1.0 ;  
        parms.position[1] = 1.0 ;  
        parms.range_start.row = 2;  
        parms.range_end.row   = 3;  
        parms.range_start.col = 1;  
        parms.range_end.col   = 2;  
        parms.use_grid_lines = TRUE;  
        parms.use_vert_grid_lines = TRUE;  
        parms.use_horiz_grid_lines = TRUE;  
        parms.use_col_hdr_grid_lines = TRUE;  
        parms.use_row_hdr_grid_lines = TRUE;  
        parms.title_cell.row = 1;  
        parms.title_cell.col = 1;  
        parms.use_title_cell = TRUE;  
        parms.auto_size_cells = FALSE;  
    /*  Create the tabular note 里海译:Create the tabular note */  
        if (!UF_CALL( UF_DRAW_create_tabular_note(&parms, &new_tab_note)))  
        {  
         /* Write some cells 里海译:以下是翻译结果：

单元格1:  在单元格1中，请写下你的想法和意见。

单元格2:  在单元格2中，你可以继续补充你的想法和意见。

单元格3:  在单元格3中，你可以写下你希望讨论的议题。

单元格4:  在单元格4中，请列出你的问题和疑问。

单元格5:  在单元格5中，请提出你的建议或解决方案。

单元格6:  在单元格6中，请写下你的总结和结论。 */  
            cell_params.ug_font = fonts[0] ;  
            cell_params.ug_text_height = 0.15625 ;  
            cell_params.horiz_just = UF_DRAW_tabnot_just_center ;  
            cell_params.vert_just = UF_DRAW_tabnot_just_middle ;  
            cell_params.text_angle = 0.0 ;  
            cell_params.is_bold = FALSE ;  
            cell_params.is_italic = FALSE ;  
            cell_params.is_underlined = FALSE ;  
            cell_params.is_wrapped = FALSE ;  
            cell_params.strikethru = FALSE ;  
            cell_params.is_hidden = FALSE ;  
            cell_params.cell_text = "TITLE OF NOTE";  
            UF_DRAW_write_tabnot_cell( new_tab_note, 1, 1, &cell_params) ;  
            cell_params.cell_text = "Feature";  
            UF_DRAW_write_tabnot_cell( new_tab_note, 2, 1, &cell_params) ;  
            cell_params.cell_text = "Diameter";  
            UF_DRAW_write_tabnot_cell( new_tab_note, 2, 2, &cell_params);  
            UF_CALL( UF_DRAW_write_tabnot_col_wdt( new_tab_note, 1, 3));  
            UF_CALL( UF_DRAW_write_tabnot_col_wdt( new_tab_note, 2, 3));  
            UF_CALL( UF_DRAW_write_tabnot_row_hgt( new_tab_note, 1, .5));  
        /*  Update the cell sizes of the tabular note 里海译:更新表格注释中的单元格大小 */  
            UF_CALL( UF_DRAW_update_tabnot( new_tab_note));  
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

> 这是用于创建NX中的表格注释的二次开发代码。具体功能包括：
>
> 1. 设置表格注释的默认参数，包括位置、范围、是否使用网格线等。
> 2. 设置标题单元格的参数，包括字体、文本高度、对齐方式等。
> 3. 创建表格注释对象。
> 4. 在表格注释中写入单元格文本，包括标题、列名等。
> 5. 设置列宽和行高。
> 6. 更新表格注释，以应用设置的列宽和行高。
> 7. 使用UF_print_syslog函数打印错误信息。
> 8. 在UFusr函数中初始化和终止NX Open API。
> 9. 设置立即卸载标志。
>
> 总体而言，这段代码实现了创建NX中的表格注释，并设置了表格注释的属性、单元格文本、列宽和行高等。
>
