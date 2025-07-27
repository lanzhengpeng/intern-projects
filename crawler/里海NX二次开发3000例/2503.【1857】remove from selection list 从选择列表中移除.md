### 【1857】remove from selection list 从选择列表中移除

#### 代码

```cpp
    /*HEAD REMOVE_FROM_SELECTION_LIST CCC UFUN */  
    /****************************************************************************  
    **  
    **  
    **  Abstract: Dialog allows selection.  Demonstrates the use of  
    **  UF_UI_remove_from_sel_list(), by putting the UF_UI_selection_p_t  
    **  selection information into a structure element which can be easily   
    **  passed into other functions.  
    **  
    **  
    ****************************************************************************/  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_curve.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
    #include <uf_object_types.h>  
    #define GTAC_CB_COUNT ( 3 + 1 )  
    #define GTAC_LABEL_0                   ("LABEL_0")  
    #define GTAC_UNHIGHLIGHT               ("UNHIGHLIGHT")  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据上下文，UF_print_syslog是在V18版本中新增的函数。 */  
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
    /* data structure to hold the selection information 里海译:数据结构以保存选择信息 */  
    typedef struct data_s   
    {   
        UF_UI_selection_p_t selection;   
    } data_t, *data_p_t;  
    /* qq3123197280 */  
    static int sel_cb ( int num_sel, tag_p_t sel_objects,   
                       int num_deselected, tag_p_t deselected_objs,  
                       void *client_data, UF_UI_selection_p_t select)  
    {  
        char  
            msg[133] = { "" };  
        tag_t     
            view,   
            point;  
        tag_p_t      
            objs;  
        double    
            pos[3];  
        int   
            descriptor,   
            inx = 0,   
            count = 0;  
        data_p_t      
            dialog_state = client_data;  
        UF_CALL(UF_UI_ask_sel_descriptor(select, &descriptor));  
        if (descriptor & UF_UI_SEL_DESC_SINGLE_POSITION)  
        {  
            UF_CALL(UF_UI_ask_sel_cursor_pos(select, &view, pos));  
            UF_CALL( UF_CURVE_create_point( pos, &point));  
        }  
        UF_CALL(UF_UI_ask_sel_object_list( select, &count, &objs ));  
        sprintf(msg, "count of ents in sel_cb : %d\n", count );  
        dialog_state->selection = select; /** copy selection info  
                                                  into structure **/  
        UF_UI_write_listing_window( msg );  
        if( count )  
        {  
            for( inx = 0; inx < count; inx++)  
            {              
                sprintf( msg, "   tag: %d\n", objs[inx] );  
                UF_UI_write_listing_window( msg );  
            }  
            UF_free( objs );  
        }  
        return( UF_UI_CB_CONTINUE_DIALOG);  
    }  
    /*---------------------- UIStyler Callback Functions ----------------------*/  
    /* qq3123197280 */  
    int GTAC_const_cb ( int dialog_id, void * client_data,  
                        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        UF_STYLER_item_value_type_t sel_data;  
        sel_data.item_id = UF_STYLER_DIALOG_INDEX;  
        sel_data.item_attr  = UF_STYLER_SELECTION;  
        UF_STYLER_ask_value ( dialog_id, &sel_data );  
        UF_CALL( UF_UI_set_sel_procs ( sel_data.value.selection,  
                                       NULL, sel_cb, client_data ));  
        return (UF_UI_CB_CONTINUE_DIALOG);   
    }  
    /* -----------------------------------------------------------------------*/  
    /* qq3123197280 */  
    int GTAC_back_cb ( int dialog_id,  
                 void * client_data,  
                 UF_STYLER_item_value_type_p_t callback_data)  
    {  
        return (UF_UI_CB_EXIT_DIALOG);   
    }  
    /* -------------------------------------------------------------------------*/  
    /* qq3123197280 */  
    int GTAC_unhighlight_cb ( int dialog_id,  
                 void * client_data,  
                 UF_STYLER_item_value_type_p_t callback_data)  
    {  
        data_p_t      
            dialog_state = client_data;  
        int  
            count = 0;  
        char  
            msg[133] = { "" };  
        tag_p_t  
            objs;           
        logical   
            unhighlight = TRUE;      
        UF_CALL(UF_UI_ask_sel_object_list( dialog_state->selection,   
                                                      &count, &objs ));  
        sprintf( msg, "unhighlight %d items\n", count );  
        UF_UI_write_listing_window( msg );  
        if (count)  
        {                                     
            UF_CALL(UF_UI_remove_from_sel_list( dialog_state->selection,  
                                                 count, objs, unhighlight));   
            UF_free( objs );  
        }  
        return (UF_UI_CB_CONTINUE_DIALOG);   
    }  
    static void do_it(void)  
    {  
        int    
            response = 0;  
        data_t      
            dialog_state;  
        static UF_STYLER_callback_info_t GTAC_cbs[GTAC_CB_COUNT] =   
        {  
          {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB, 0, GTAC_const_cb},  
          {UF_STYLER_DIALOG_INDEX, UF_STYLER_BACK_CB, 0, GTAC_back_cb},  
          {GTAC_UNHIGHLIGHT, UF_STYLER_ACTIVATE_CB, 1, GTAC_unhighlight_cb},  
          {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 }  
        };  
        UF_CALL( UF_UI_open_listing_window() );  
        UF_CALL( UF_STYLER_create_dialog ( "rem_sel_list.dlg",  
               GTAC_cbs,        /* Callbacks from dialog 里海译:对话中的回调翻译为："来自对话的回调"。 */  
               GTAC_CB_COUNT,   /* number of callbacks*/  
               &dialog_state,   /* This is your client data 里海译:这是您的客户资料。 */  
               &response ));  
        return;  
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

> 这段代码是一个用于NX的二次开发示例，其主要功能如下：
>
> 1. 选择对象：使用UF_UI_set_sel_procs函数设置选择回调，当用户在对话框中选择对象时，会触发sel_cb回调函数，该函数会获取选择的对象信息，并将其存储在dialog_state结构体中。
> 2. 反高亮对象：GTAC_unhighlight_cb回调函数会获取dialog_state结构体中存储的选择对象，并使用UF_UI_remove_from_sel_list函数将这些对象从选择列表中移除，实现反高亮功能。
> 3. 对话框回调：代码中定义了3个对话框回调函数，包括初始化回调GTAC_const_cb、返回回调GTAC_back_cb和反高亮回调GTAC_unhighlight_cb。这些回调函数分别用于设置选择回调、返回和反高亮对象。
> 4. 错误处理：使用UF_CALL宏进行错误处理，该宏会检查函数调用返回码，如果有错误则输出错误信息。
> 5. 主函数：在ufusr主函数中调用do_it函数以创建对话框并处理用户操作。在操作结束后，使用UF_terminate终止NX会话。
> 6. 卸载函数：提供了ufusr_ask_unload函数以实现立即卸载二次开发程序。
>
> 这段代码通过对话框实现了选择和反高亮对象的功能，展示了NX二次开发中对话框、选择和错误处理等常用技术的使用方法。
>
