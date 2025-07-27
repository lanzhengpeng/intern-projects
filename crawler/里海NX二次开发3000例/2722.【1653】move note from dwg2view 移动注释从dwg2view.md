### 【1653】move note from dwg2view 移动注释从dwg2view

#### 代码

```cpp
    /*HEAD MOVE_NOTE_FROM_DWG2VIEW CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_view.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是在V18版本中新增的函数。 */  
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
    static void select_a_view( char *prompt, char *view_name)  
    {  
        int  
            ret_code,  
            any_view = 0,  
            work_view = 1,  
            view_of_cursor;  
        double  
            pos[3];  
        /* Read view of cursor (work or any?) 里海译:游标的工作视图或任何视图的读视图 */  
        UF_CALL( UF_UI_ask_cursor_view( &view_of_cursor));  
        UF_CALL( UF_UI_set_cursor_view( any_view));  
        ret_code = uc1615(prompt, pos);  
        if( view_of_cursor == work_view )  
        /* Set view of cursor back to original 里海译:将光标视图恢复到原始位置 */  
            UF_CALL(UF_UI_set_cursor_view( work_view));  
        if (ret_code == 5)  
        {  
            /* Return view name of last position indication 里海译:返回上次位置指示的视图名称 */  
            UF_CALL( uc1653( view_name ));  
        }  
        else strcpy(view_name, "");  
    }  
    /* qq3123197280 */  
    static int mask_for_notes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_drafting_entity_type, UF_draft_note_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_note(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog("Select a note", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_notes, NULL, &resp,  
            &object, cp, &view));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        char   
            view_name[31]="";  
        tag_t  
            note;  
        select_a_view("Select destination view", view_name);  
        printf("\n The View Name is: %s\n",view_name);  
        note = select_a_note("Select note to move");  
        UF_CALL(uc6401 ("", note));  
        UF_CALL(uc6402 (view_name, note));  
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

> 这段代码是用于NX的二次开发，主要实现了以下功能：
>
> 1. 错误报告功能：通过宏定义实现了错误报告函数report_error，用于打印出函数调用失败时的错误信息。
> 2. 视图选择功能：实现了select_a_view函数，用于提示用户选择一个视图，并返回视图名称。
> 3. 注释选择功能：实现了mask_for_notes函数，用于设置选择过滤器，仅选择注释类型的对象；同时实现了select_a_note函数，用于提示用户选择一个注释对象，并返回注释对象的标签。
> 4. 注释移动功能：实现了do_it函数，用于选择一个视图和一个注释对象，并调用UF函数将注释对象移动到选择的视图中。
> 5. 主函数：ufusr函数初始化NX环境，调用do_it函数执行注释移动功能，然后终止NX环境。
> 6. 卸载函数：ufusr_ask_unload函数用于立即卸载用户自定义函数。
>
> 总体来说，这段代码实现了在NX中通过二次开发实现选择视图和注释对象，并将注释对象移动到指定视图的功能。代码结构清晰，功能明确。
>
