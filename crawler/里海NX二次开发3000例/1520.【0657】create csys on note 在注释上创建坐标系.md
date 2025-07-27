### 【0657】create csys on note 在注释上创建坐标系

#### 代码

```cpp
    /*HEAD CREATE_CSYS_ON_NOTE CCC UFUN */  
    #include <uf_csys.h>  
    #include <uf_drf.h>  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog 是 V18 中新增的功能，用于打印系统日志。 */  
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
    /* 里海 */  
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
        tag_t   matrix, note, csys_id;  
        int     num_strings;  
        UF_DRF_draft_aid_text_info_t    *text_info;  
        note = select_a_note("Select note");  
        UF_CALL( UF_CSYS_ask_matrix_of_object( note, &matrix));  
        UF_CALL( UF_DRF_ask_draft_aid_text_info( note, &num_strings, &text_info));  
        UF_CALL( UF_CSYS_create_csys( text_info->origin, matrix, &csys_id));  
        UF_CALL(UF_DRF_free_text(num_strings, &text_info));  
    }  
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

> 这段代码实现了在NX中创建一个坐标系的功能，具体步骤如下：
>
> 1. 头文件包含：引入了必要的NX Open API头文件。
> 2. 错误处理函数：定义了一个错误处理函数report_error，用于在函数调用失败时输出错误信息。
> 3. 选择注释：定义了一个函数mask_for_notes，用于在对话框中选择注释实体。
> 4. 选择注释实体：定义了一个函数select_a_note，用于选择一个注释实体，并返回其tag。
> 5. 创建坐标系：定义了一个函数do_it，用于在注释的文本原点处创建一个坐标系。首先获取注释的变换矩阵，然后获取注释的文本信息，最后在文本原点处创建坐标系。
> 6. ufusr函数：在NX中启动时调用ufusr函数，执行do_it函数来创建坐标系。
> 7. 卸载函数：定义了一个卸载函数ufusr_ask_unload，在NX关闭时调用，立即卸载该二次开发代码。
>
> 总的来说，这段代码提供了在NX中注释文本原点处创建坐标系的功能。
>
