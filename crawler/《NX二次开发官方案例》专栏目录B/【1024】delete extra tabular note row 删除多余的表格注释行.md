### 【1024】delete extra tabular note row 删除多余的表格注释行

#### 代码

```cpp
    /*HEAD DELETE_EXTRA_TABULAR_NOTE_ROW CCC UFUN */  
    /*  
        This program demonstrates deleting a single extra row in a selected  
        tabular note in NX 1.  
        If an NX 1 part has a tabular note with cells outside the visible  
        range of the note, the tabular note will automatically be  
        associated to a spreadsheet when the part is upgraded to NX 2 to  
        prevent lossing these extra cells.  To fix the part so that the  
        tabular note cells can be edited without connecting to the  
        spreadsheet, these extra out-of-bound cells must be deleted in NX 1  
        before the part is upgraded to NX 2.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的函数。 */  
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
            ii;  
        tag_t  
            tnote;  
        UF_DRAW_tabnot_params_t  
            params;  
        UF_DRAW_tabnot_eval_data_t  
            eval;  
        while ((tnote = select_a_tabular_note("Remove extra row")) != NULL_TAG)  
        {  
            UF_CALL(UF_DRAW_ask_tabular_note_params(tnote, &params, &eval));  
            for (ii = 1; ii <= params.range_end.col; ii++)  
            {  
                UF_CALL(UF_DRAW_delete_tabnot_cell(tnote, params.range_end.row+1,  
                    ii));  
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

> 这段代码是NX的二次开发代码，主要功能是删除NX 1零件中选定的表格注释中的额外行。在NX 1中，如果表格注释包含超出可见范围的单元格，在升级到NX 2时，这些单元格会自动关联到电子表格以避免丢失。为了在升级后能够编辑表格注释的单元格，需要在升级前删除这些额外的单元格。代码的主要流程包括：
>
> 1. 包含必要的头文件。
> 2. 定义一个错误报告函数report_error，用于打印函数调用失败的信息。
> 3. 定义一个选择表格注释的函数select_a_tabular_note，用于让用户选择一个表格注释，并返回其tag。
> 4. 定义主体函数do_it，用于处理选中的表格注释，删除其最后一行。
> 5. 定义UFusr入口函数，初始化后调用do_it函数，最后终止。
> 6. 定义卸载函数ufusr_ask_unload，返回立即卸载。
> 7. 整体流程为：用户选择一个表格注释，代码删除该注释的最后一行，循环此过程直到用户选择“取消”。在NX 1中运行此代码可以删除表格注释的额外行，避免升级到NX 2后出现关联电子表格的问题。
>
