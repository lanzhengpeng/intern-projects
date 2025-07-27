### 【1826】read tabular note cell text 读取表格注释单元格文本

#### 代码

```cpp
    /*HEAD REPORT_TABULAR_NOTE_TEXT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据原文，翻译如下：

注意：UF_print_syslog 是 V18 中新增的。 */  
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
            jj;  
        tag_t  
            tabular_note;  
        UF_DRAW_tabnot_params_t  
            tn_parms;  
        UF_DRAW_tabnot_eval_data_t  
            eval_data;  
        UF_DRAW_tabnot_cell_params_t  
            cell_params;  
        UF_DRAW_tabnot_cell_eval_data_t  
            cel_eval_data;  
        char  
            msg[133],  
            sizemsg[100];  
        while ((tabular_note = select_a_tabular_note("Select a tabular note"))  
                != NULL_TAG)  
        {  
            UF_CALL(UF_DRAW_ask_tabular_note_params(tabular_note, &tn_parms,  
                &eval_data));  
            sprintf(sizemsg, "There are %d rows (%d - %d) and %d cols (%d - %d)",  
            (tn_parms.range_end.row - tn_parms.range_start.row + 1),  
            tn_parms.range_start.row, tn_parms.range_end.row,  
            (tn_parms.range_end.col - tn_parms.range_start.col + 1),  
            tn_parms.range_start.col, tn_parms.range_end.col);  
            uc1601(sizemsg, TRUE);  
            UF_UI_open_listing_window();  
            for (ii = tn_parms.range_start.row;  
                    ii <= tn_parms.range_end.row; ii++)  
            {  
                for (jj = tn_parms.range_start.col;  
                        jj <= tn_parms.range_end.col; jj++)  
                {  
                    UF_CALL(UF_DRAW_read_tabnot_cell(tabular_note, ii, jj,  
                        &cell_params, &cel_eval_data));  
                    sprintf(msg, "[%d][%d]Text: %s\t ", ii, jj, cell_params.cell_text);  
                    UF_UI_write_listing_window(msg);  
                }  
                UF_UI_write_listing_window("\n");  
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

> 这段NX二次开发代码的主要功能是选择表格注释对象，并读取其内容，具体步骤如下：
>
> 1. 定义错误报告函数report_error，用于打印错误信息。
> 2. 定义选择表格注释对象的函数select_a_tabular_note，使用对话框让用户选择一个表格注释对象，并返回其tag。
> 3. 定义读取表格注释内容的函数do_it，遍历每个单元格，读取其文本内容，并打印在列表窗口中。
> 4. 定义UFusr函数，初始化NX，调用do_it函数，然后终止NX。
> 5. 定义ufusr_ask_unload函数，设置用户退出时立即卸载二次开发代码。
>
> 该代码通过选择表格注释对象，读取并打印其内容，实现了对NX表格注释的二次开发操作。
>
