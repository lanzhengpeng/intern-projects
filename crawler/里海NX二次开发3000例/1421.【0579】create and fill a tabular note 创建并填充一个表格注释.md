### 【0579】create and fill a tabular note 创建并填充一个表格注释

#### 代码

```cpp
    /*HEAD CREATE_AND_FILL_A_TABULAR_NOTE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
    #include <uf_drf.h>  
    #include <uf_disp.h>  
    #include <uf_tabnot.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本中的新功能。 */  
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
    static logical specify_point(char *prompt, double loc[3])  
    {  
        int  
            ia2[2] = { 0,0 },  
            ip3 = 0,  
            irc;  
        irc = uc1616(prompt, ia2, ip3, loc);  
        if (irc == 5) return TRUE;  
        else return FALSE;  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,jj;  
        tag_t  
            section,  
            tabular_note;  
        double  
            location[3],  
            col_wdts[4] = { 1.5, 2, 2.5, 3 };  
        char  
            cell_text[133];  
        UF_DRAW_tabnot_params_t  
            tab_note_params;  
        UF_DRAW_tabnot_cell_params_t  
            cell_params;  
    /*  Just in case we're not setting everything possible here 译:以防万一我们在这里没有设置一切可能的事情。 */  
        UF_CALL(UF_DRAW_ask_tabular_note_defaults(&tab_note_params));  
        tab_note_params.title_cell.row = 1;  
        tab_note_params.title_cell.col = 2;  
        tab_note_params.range_start.row = 2;  
        tab_note_params.range_end.row = 5 + 1;  /* 1 more than actual 译:翻译为：比实际更重要 */  
        tab_note_params.range_start.col = 1;  
        tab_note_params.range_end.col = 2;  
        tab_note_params.ug_aspect_ratio = 1.0;  
        tab_note_params.border_type = UF_DRAW_tabnot_border_type_single;  
        tab_note_params.border_width = 1;  
        tab_note_params.use_title_cell = TRUE;  
        tab_note_params.use_grid_lines = TRUE;  
        tab_note_params.use_vert_grid_lines = TRUE;  
        tab_note_params.use_horiz_grid_lines = TRUE;  
        tab_note_params.use_row_hdr_grid_lines = TRUE;  
        tab_note_params.use_col_hdr_grid_lines = TRUE;  
        tab_note_params.auto_size_cells = FALSE;  
        cell_params.cell_text = cell_text;  
        cell_params.ug_font = "Blockfont";  
        cell_params.ug_text_height = 0.125;  
        cell_params.horiz_just = UF_DRAW_tabnot_just_center;  
        cell_params.vert_just = UF_DRAW_tabnot_just_middle;  
        cell_params.text_angle = 0;  
        cell_params.is_bold = FALSE;  
        cell_params.is_italic = FALSE;  
        cell_params.is_underlined = FALSE;  
        cell_params.is_wrapped = FALSE;  
        cell_params.strikethru = FALSE;  
        cell_params.is_hidden = FALSE;  
        if (specify_point("Indicate origin", tab_note_params.position))  
        {  
            UF_CALL(UF_DRAW_create_tabular_note(&tab_note_params, &tabular_note));  
            strcpy(cell_params.cell_text, "Title Cell");  
            WRITE(cell_params.cell_text);  
            WRITE("\n");  
            UF_CALL(UF_DRAW_write_tabnot_cell(tabular_note,  
                tab_note_params.title_cell.row, tab_note_params.title_cell.col,  
                &cell_params));  
            for (jj = tab_note_params.range_start.col;  
                    jj <= tab_note_params.range_end.col; jj++)  
            {  
                for (ii = tab_note_params.range_start.row;  
                        ii < tab_note_params.range_end.row; ii++)  
                {  
                    sprintf(cell_params.cell_text, "Cell %d,%d", ii, jj);  
                    WRITE(cell_params.cell_text);  
                    WRITE("\n");  
                    UF_CALL(UF_DRAW_write_tabnot_cell(tabular_note, ii, jj,  
                        &cell_params));  
                }  
                UF_CALL(UF_DRAW_write_tabnot_col_wdt(tabular_note, jj,  
                    col_wdts[jj-1]));  
            }  
            UF_CALL(UF_DRAW_update_tabnot(tabular_note));  
            UF_CALL(UF_TABNOT_ask_nth_section(tabular_note, 0, &section));  
            UF_CALL(UF_OBJ_set_layer(section, 2));  
            UF_CALL(UF_DRF_ask_origin(section, location));  
            display_temporary_point(location);  
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

> 根据代码内容，这是一段用于在NX中创建和填充表格注释的二次开发代码。以下是代码的主要功能和逻辑：
>
> 1. 包含了NX二次开发常用的头文件，如uf.h, uf_ui.h等。
> 2. 定义了错误报告函数report_error，用于打印错误信息。
> 3. 定义了指定点坐标的函数specify_point，通过UI提示用户输入点坐标。
> 4. 定义了显示临时点的函数display_temporary_point。
> 5. 主函数do_it实现了创建表格注释的逻辑：获取表格注释的默认参数。设置表格注释的位置、范围、字体等参数。创建表格注释对象。在表格注释中写入标题单元格。遍历所有单元格，写入文本内容。设置列宽。更新表格注释。显示表格注释的原点位置。
> 6. 获取表格注释的默认参数。
> 7. 设置表格注释的位置、范围、字体等参数。
> 8. 创建表格注释对象。
> 9. 在表格注释中写入标题单元格。
> 10. 遍历所有单元格，写入文本内容。
> 11. 设置列宽。
> 12. 更新表格注释。
> 13. 显示表格注释的原点位置。
> 14. ufusr函数是NX二次开发的入口函数，初始化后调用do_it，最后终止。
> 15. ufusr_ask_unload函数用于设置立即卸载二次开发程序。
>
> 总的来说，这段代码通过UI交互创建了一个自定义的表格注释，并在其中填充了文本内容，是一个典型的NX二次开发示例。
>
