### 【2682】select only one object with deselect and ok 选择一个对象，取消其他选择，然后确定

#### 代码

```cpp
    /*HEAD SELECT_ONLY_ONE_OBJECT_WITH_DESELECT_AND_OK CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
    static int select_only_one_cb(  
            int num_selected,  
            tag_p_t  selected_objects,  
            int num_deselected,  
            tag_p_t  deselected_objects,  
            void *user_data,  
            UF_UI_selection_p_t select)  
    {  
        int  
            count;  
        tag_t  
            *objs;  
        UF_CALL(UF_UI_ask_sel_object_list(select, &count, &objs));  
        if (count > 1)  
        {  
            UF_CALL(UF_UI_remove_from_sel_list(select, count - 1, objs, TRUE));  
            UF_free(objs);  
        }  
        return UF_UI_CB_CONTINUE_DIALOG;  
    }  
    /* qq3123197280 */  
    static mask_for_one_object(UF_UI_selection_p_t select, void *type)  
    {  
        if (!UF_CALL(UF_UI_set_sel_procs(select, NULL, select_only_one_cb, NULL)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_one_object(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            object,  
            *objects;  
        UF_CALL(UF_UI_select_with_class_dialog("Select One Object", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_one_object, NULL, &resp,  
            &cnt, &objects));  
        if (cnt <= 0) return NULL_TAG;  
        object = objects[0];  
        UF_free(objects);  
        UF_CALL(UF_DISP_set_highlight(object, FALSE));  
        return object;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            object;  
        while ((object = select_one_object("Select any object")) != NULL_TAG)  
        {  
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

> 这段代码是一个NX Open C++应用程序，其主要功能是选择一个NX对象，并实现以下关键功能：
>
> 1. 错误报告：使用宏ECHO和UF_PRINT_SYSLOG打印错误信息。
> 2. 选择回调：使用select_only_one_cb函数，确保只选择一个对象，如果选择了多个对象，它会取消选择最后一个。
> 3. 选择掩码：使用mask_for_one_object函数设置选择回调，以实现只选择一个对象的功能。
> 4. 选择对象：使用select_one_object函数打开选择对话框，并返回选择的对象。
> 5. 主循环：在do_it函数中循环调用select_one_object，直到用户取消选择。
> 6. 初始化和终止：在ufusr函数中初始化和终止NX Open。
> 7. 卸载函数：提供卸载函数ufusr_ask_unload。
>
> 整体来看，这段代码实现了在NX中选择单个对象的功能，通过回调函数确保每次只选择一个对象，并通过主循环让用户可以反复选择，直到取消选择为止。
>
