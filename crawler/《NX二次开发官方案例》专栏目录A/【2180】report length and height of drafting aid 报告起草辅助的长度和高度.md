### 【2180】report length and height of drafting aid 报告起草辅助的长度和高度

#### 代码

```cpp
    /*HEAD REPORT_LENGTH_AND_HEIGHT_OF_DRAFTING_AID CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的函数，用于打印系统日志信息。文档要求只提供翻译，不要添加其他无关内容。因此，翻译结果为：

UF_print_syslog是V18版本中新增的函数，用于打印系统日志信息。 */  
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
        UF_CALL(UF_UI_select_with_single_dialog("Select a note", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_notes, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        int    
            num_text;  
        tag_t  
            ug_note = NULL_TAG;  
        UF_DRF_draft_aid_text_info_t    
            *text_info;  
        while( (ug_note = select_a_note( "Pick a Note:" )) != NULL_TAG )  
        {  
            UF_CALL(UF_DRF_ask_draft_aid_text_info ( ug_note,   
                                                     &num_text, &text_info ));  
            WRITE_F( text_info->height );  
            WRITE_F( text_info->length );  
            UF_UI_write_listing_window( "\n" );  
            UF_DRF_free_text( num_text, &text_info );  
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

> 这段代码是NX的二次开发示例，其主要功能是查询并报告标注辅助中的文本对象的高度和长度。
>
> 主要步骤包括：
>
> 1. 初始化UF系统。
> 2. 定义错误报告函数report_error，用于打印错误信息。
> 3. 定义mask_for_notes函数，设置选择过滤器，只选择标注类型的对象。
> 4. 定义select_a_note函数，通过对话框让用户选择一个标注对象。
> 5. 定义WRITE_F宏，用于将浮点数打印到列表窗口。
> 6. 定义do_it函数，循环调用select_a_note选择标注对象，然后调用UF_DRF_ask_draft_aid_text_info查询文本信息，并打印高度和长度。
> 7. ufusr函数作为主入口，调用do_it函数。
> 8. ufusr_ask_unload函数声明不需要卸载。
>
> 该代码通过用户交互选择标注对象，并查询其文本的高度和长度信息，实现了简单的标注信息获取功能。
>
