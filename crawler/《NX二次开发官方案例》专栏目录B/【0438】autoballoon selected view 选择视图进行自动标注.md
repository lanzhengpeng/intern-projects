### 【0438】autoballoon selected view 选择视图进行自动标注

#### 代码

```cpp
    /*HEAD AUTOBALLOON_SELECTED_VIEW CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_plist.h>  
    #include <uf_tabnot.h>  
    #include <uf_undo.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog 是 V18 版本中新增的功能。 */  
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
    static int mask_for_parts_list(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_tabular_note_type, UF_parts_list_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_parts_list(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_parts_list, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static tag_t select_drawing_member_view(char *prompt)  
    {  
        int  
            orig_cv,  
            resp;  
        tag_t  
            view_tag;  
        double  
            pos[3];  
        UF_CALL(UF_UI_ask_cursor_view(&orig_cv));  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_specify_screen_position(prompt, NULL, NULL, pos,  
            &view_tag, &resp));  
        UF_CALL(UF_UI_set_cursor_view(orig_cv));  
        if (resp == UF_UI_PICK_RESPONSE)  
            return view_tag;  
        else  
            return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            plist,  
            view;  
        UF_PLIST_prefs_t  
            prefs;  
        UF_UNDO_mark_id_t  
            mark;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, "Autoballoon view", &mark));  
        if (((plist = select_a_parts_list("Autoballon")) != NULL_TAG) &&  
            ((view = select_drawing_member_view("Autoballoon")) != NULL_TAG))  
        {  
            UF_CALL(UF_TABNOT_ask_tabular_note_of_section(plist, &plist));  
            UF_CALL(UF_PLIST_ask_prefs(plist, &prefs));  
            if (prefs.symbol_type == UF_PLIST_symbol_type_none)  
            {  
                prefs.symbol_type = UF_PLIST_symbol_type_id_symbol_circle;  
                UF_CALL(UF_PLIST_set_prefs(plist, &prefs));  
            }  
            UF_CALL(UF_PLIST_add_object(plist, FALSE, view));  
            UF_CALL(UF_PLIST_update(plist));  
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

> 这段代码是一个NX Open的UF程序，主要实现了以下功能：
>
> 1. 提供了一个错误报告函数report_error，用于在出错时输出错误信息到syslog和listing window。
> 2. 定义了一个选择部件清单的函数mask_for_parts_list，设置了选择部件清单的过滤条件。
> 3. 提供了选择部件清单和视图的函数select_a_parts_list和select_drawing_member_view。
> 4. do_it函数实现了主要功能：选择部件清单和视图，设置部件清单的偏好，将视图添加到部件清单中并更新。
> 5. ufusr函数是UF程序的入口函数，初始化后调用do_it函数，最后结束。
> 6. ufusr_ask_unload函数指定了当程序卸载时立即卸载。
>
> 整体来看，这段代码实现了选择部件清单和视图，并将视图添加到部件清单中的功能，用于自动标记视图。
>
