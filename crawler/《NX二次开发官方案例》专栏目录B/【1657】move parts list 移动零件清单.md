### 【1657】move parts list 移动零件清单

#### 代码

```cpp
    /*HEAD MOVE_PARTS_LIST CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是在V18版本中新增的函数。 */  
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
    static int mask_for_parts_list(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_tabular_note_type, 0, 0 };  
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
    static logical select_pos(char *prompt, double pos[3])  
    {  
        int  
            resp = uc1615(prompt, pos);  
        if (resp == 5) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            plist;  
        double  
            pos[3];  
        while (((plist =  
            select_a_parts_list("Select parts list to move")) != NULL_TAG) &&  
            select_pos("Indicate new location", pos))  
        {  
            UF_CALL(UF_DRF_set_origin(plist, pos));  
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

> 这段NX二次开发代码实现了移动零件清单的功能。以下是代码的主要功能和实现：
>
> 1. 错误报告函数：report_error函数用于在出错时输出错误信息和调用信息，有助于定位问题。
> 2. 零件清单选择函数：select_a_parts_list函数使用对话框让用户选择一个零件清单，并使用mask_for_parts_list函数来过滤只选择零件清单。
> 3. 位置输入函数：select_pos函数提示用户输入位置，并将输入的坐标存储在pos数组中。
> 4. 主要执行函数：do_it函数使用循环调用上述两个函数，让用户选择零件清单和输入位置，然后使用UF_DRF_set_origin函数移动零件清单到新位置。
> 5. 入口函数：ufusr是NX二次开发的入口函数，其中调用UF_initialize进行初始化，然后调用do_it执行功能，最后使用UF_terminate进行终止。
> 6. 卸载函数：ufusr_ask_unload用于设置卸载方式，这里设置为立即卸载。
>
> 总的来说，这段代码通过对话框和用户交互，实现了在NX中移动零件清单的功能。代码结构清晰，实现了选择、输入、移动的完整流程，是一个典型的NX二次开发示例。
>
