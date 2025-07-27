### 【2796】set tabular note text size 设置表格注释文本大小

#### 代码

```cpp
    /*HEAD SET_TABULAR_NOTE_TEXT_SIZE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog是V18版本新增的功能，请仅提供翻译，不要添加其他无关内容。 */  
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
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    /* qq3123197280 */  
    static int mask_for_tabular_notes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_tabular_note_type, 0, 0 };  
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
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_tabular_notes, NULL, &resp,  
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
            ii,  
            jj,  
            units;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            tabular_note;  
        double  
            fsize = .125;  
        UF_DRAW_tabnot_params_t  
            tn_parms;  
        UF_DRAW_tabnot_eval_data_t  
            eval_data;  
        UF_DRAW_tabnot_cell_params_t  
            cell_params;  
        UF_DRAW_tabnot_cell_eval_data_t  
            cel_eval_data;  
        char  
            sizemsg[100];  
        UF_CALL(UF_PART_ask_units(part, &units));  
        if(units == 1) fsize = 3.75;  
        if (prompt_for_a_number("Tabular Note Font Size", "Font Size", &fsize))  
        {  
            while ((tabular_note = select_a_tabular_note("Select a tabular note"))  
                    != NULL_TAG)  
            {  
                UF_CALL(UF_DRAW_ask_tabular_note_params(tabular_note, &tn_parms,  
                    &eval_data));  
                sprintf(sizemsg, "There are %d rows and %d cols",  
                (tn_parms.range_end.row - tn_parms.range_start.row + 1),  
                (tn_parms.range_end.col - tn_parms.range_start.col + 1));  
                uc1601(sizemsg, FALSE);  
                for (ii = tn_parms.range_end.row;  
                        ii >= tn_parms.range_start.row; ii--)  
                    for (jj = tn_parms.range_end.col;  
                            jj >= tn_parms.range_start.col; jj--)  
                    {  
                        UF_CALL(UF_DRAW_read_tabnot_cell(tabular_note, ii, jj,  
                            &cell_params, &cel_eval_data));  
                        cell_params.ug_text_height = fsize;  
                        UF_CALL(UF_DRAW_write_tabnot_cell(tabular_note, ii, jj,  
                            &cell_params));  
                    }  
                tn_parms.auto_size_cells = FALSE;  
                UF_CALL(UF_DRAW_set_tabular_note_params(tabular_note, &tn_parms));  
                UF_CALL(UF_DRAW_update_tabnot(tabular_note));  
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

> 这段代码是用于设置NX中表格注释文字大小的二次开发代码。
>
> 主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于在调用UF函数失败时输出错误信息。
> 2. 定义了一个提示用户输入数字的函数prompt_for_a_number，用于获取表格注释文字的大小。
> 3. 定义了一个用于选择表格注释的函数select_a_tabular_note，通过对话框让用户选择一个表格注释。
> 4. 在do_it函数中，首先获取当前部件的单位，如果是英寸单位，则默认文字大小设置为3.75，如果是毫米单位，则设置为0.125。
> 5. 使用prompt_for_a_number函数提示用户输入文字大小，如果用户输入了，则循环处理所有选中的表格注释。
> 6. 对于每个表格注释，先获取其参数，然后遍历每个单元格，设置单元格的文字大小为新输入的大小。
> 7. 最后更新表格注释的参数，关闭自动调整单元格大小的功能，并更新显示。
> 8. ufusr函数是程序的入口函数，首先初始化UF系统，然后调用do_it函数执行主体功能，最后终止UF系统。
> 9. ufusr_ask_unload函数用于在卸载时返回立即卸载标志。
>
> 综上所述，这段代码实现了通过对话框选择表格注释，并设置其文字大小的功能。
>
