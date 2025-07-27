### 【2671】select faces and edges with uistyler 使用UIStyler选择面和边

#### 代码

```cpp
    /*HEAD SELECT_FACES_AND_EDGES_WITH_UISTYLER CCC UFUN */  
    /* For use with this code you need to create an UI/Styler dialog  
    - change prefix name to GTAC  
    - turn on selection with type 'Robust'  
    - you may change the scope to 'Any in Assembly'  
    - no additional callbacks needed  
    */  
    #include <uf.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <stdio.h>   
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_styler.h>  
    #include <uf_mb.h>   
    #include <uf_exit.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
    #define GTAC_CB_COUNT ( 1 + 1 ) /* Add 1 for the terminator 里海译:增加1作为终结符 */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译如下：

请注意，UF_print_syslog是在V18版本中新增的，请直接回答译文，无需其他废话。 */  
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
    static int report_object(int num_sel, tag_t *sel_objects, int num_deselected,  
        tag_t *deselected_objs,  void *user_data, UF_UI_selection_p_t select)  
    {  
        int   
            ii,  
            type,  
            subtype;  
        char  
            messg[100];  
        for(ii=0; ii< num_sel; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(sel_objects[ii], &type, &subtype));  
            sprintf(messg, "Object Tag %d, Type %d, Subtype %d\n", sel_objects[ii], type, subtype);  
            if (!UF_UI_open_listing_window())  
                UF_UI_write_listing_window(messg);  
        }  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    int GTAC_sel_cb ( int dialog_id, void * client_data,  
                       UF_STYLER_item_value_type_p_t callback_data)  
    {  
        UF_STYLER_item_value_type_t    
            sel_data;  
        UF_UI_mask_t  
            mask[2] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE } };  
        sel_data.item_id = UF_STYLER_DIALOG_INDEX;  
        sel_data.item_attr  = UF_STYLER_SELECTION;  
        UF_STYLER_ask_value(dialog_id, &sel_data);  
        UF_CALL(UF_UI_set_sel_mask(sel_data.value.selection,  
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask));  
        UF_CALL(UF_UI_set_sel_procs(sel_data.value.selection, NULL,  
            report_object, NULL));  
        return (UF_UI_CB_CONTINUE_DIALOG);   
    }  
    static void do_it(void)  
    {  
        int    
            response   = 0;  
        UF_STYLER_item_value_type_t    
            selection;  
        UF_STYLER_callback_info_t GTAC_cbs[GTAC_CB_COUNT] =   
        {  
            {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB  , 0, GTAC_sel_cb},  
            {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 }   
        };  
        UF_CALL( UF_STYLER_create_dialog ( "selection.dlg",  
               GTAC_cbs, GTAC_CB_COUNT, &selection, &response ) );  
    }  
    /* qq3123197280 */      
    extern void ufusr(char *param, int *retcode, int paramLen)  
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

> 这是段NX二次开发代码，主要实现的功能是使用UI/Styler对话框来选择实体和边线，并打印出选择的类型和子类型。以下是该代码的主要流程：
>
> 1. 包含了必要的NX Open头文件，定义了错误报告函数report_error()，用于打印出错误信息和调用信息。
> 2. 定义了report_object()函数，用于打印出选择的对象的标签、类型和子类型。
> 3. 定义了GTAC_sel_cb()回调函数，用于设置选择掩码，只允许选择实体和边线，并设置report_object()为选择回调函数。
> 4. do_it()函数用于创建UI/Styler对话框，设置了GTAC_sel_cb()作为构造器回调函数。
> 5. ufusr()是NX二次开发的主函数，用于初始化NX Open、调用do_it()执行操作，然后终止NX Open。
> 6. ufusr_ask_unload()函数用于立即卸载二次开发程序。
>
> 总体来说，这段代码通过UI/Styler对话框实现了对实体和边线的选择，并打印出了选择的详细信息。代码结构清晰，功能明确，具有一定的参考价值。
>
