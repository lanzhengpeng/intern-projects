### 【1164】edit selected tabular note column widths 编辑选定表格注释的列宽

#### 代码

```cpp
    /*HEAD EDIT_SELECTED_TABULAR_NOTE_COLUMN_WIDTHS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_object_types.h>  
    #include <uf_tabnot.h>  
    #include <uf_plist.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
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
    static logical prompt_for_an_integer(char *prompt, char *item, int *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        int  
            da[1];  
        strncpy(&menu[0][0], item, 15);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
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
        strncpy(&menu[0][0], item, 16);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            n = 1;  
        tag_t  
            column,  
            section,  
            tabnot;  
        double  
            width;  
        UF_CALL(UF_TABNOT_enable_automatic_update(TRUE));  
        if ((section = select_a_tabular_note("Edit column widths")) != NULL_TAG)  
        {  
            while (prompt_for_an_integer("Edit column width", "Column No.", &n))  
            {  
                UF_CALL(UF_TABNOT_ask_tabular_note_of_section(section, &tabnot));  
                if (UF_CALL(UF_TABNOT_ask_nth_column(tabnot, n-1, &column)))  
                    continue;  
                if (!UF_CALL(UF_TABNOT_ask_column_width(column, &width)) &&  
                    prompt_for_a_number("Edit column width", "Width", &width))  
                {  
                    UF_CALL(UF_TABNOT_set_column_width(column, width));  
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

> 这段代码主要用于编辑表格注释中的列宽。主要功能包括：
>
> 1. 初始化和终止：代码开始和结束时分别调用UF_initialize()和UF_terminate()来初始化和终止NX。
> 2. 选择表格注释：通过select_a_tabular_note()函数，使用对话框让用户选择一个表格注释。
> 3. 编辑列宽：使用while循环，用户可以持续输入列号，然后编辑对应的列宽。首先通过UF_TABNOT_ask_nth_column()获取指定列的列对象，然后通过UF_TABNOT_ask_column_width()获取列宽，并让用户输入新的宽度值。如果用户输入了新的宽度值，则调用UF_TABNOT_set_column_width()设置新的列宽。
> 4. 错误处理：在关键函数调用前后使用UF_CALL宏来捕获并报告错误。
> 5. 用户输入：使用对话框获取用户输入的整数和实数。
> 6. 自动更新：在开始编辑前调用UF_TABNOT_enable_automatic_update()来开启自动更新功能。
>
> 综上所述，这段代码实现了选择表格注释、输入列号、编辑列宽等功能的二次开发，使用户可以方便地调整表格注释的列宽。
>
