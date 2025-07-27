### 【3018】unerase all objects erased in the work view 恢复工作视图中所有删除的对象

#### 代码

```cpp
    /*HEAD UNERASE_ALL_OBJECTS_ERASED_IN_THE_WORK_VIEW CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是 V18 中新增的函数。 */  
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
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void unerase_all_erased_objects(char *view)  
    {  
        int  
            irc;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            erased_list,  
            temp;  
        UF_CALL(UF_MODL_create_list(&erased_list));  
        while (((irc = uc6457(view, 3, &object)) == 0) && (object != NULL_TAG))  
            UF_CALL(UF_MODL_put_list_item(erased_list, object));  
        for (temp = erased_list; temp->next != NULL_TAG; temp = temp->next)  
            WRITENZ(uc6403(view, temp->eid));  
        UF_CALL(UF_MODL_delete_list(&erased_list));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            work_view;  
        char  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_VIEW_ask_work_view(&work_view));  
        UF_CALL(UF_OBJ_ask_name(work_view, view_name));  
        unerase_all_erased_objects(view_name);  
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

> 这段代码是一个NX Open的UF程序，主要用于实现以下功能：
>
> 1. 定义错误报告函数report_error，用于打印错误信息到系统日志和列表窗口。
> 2. 定义整数写入列表窗口的函数write_integer_to_listing_window。
> 3. 定义反擦除所有被擦除对象的函数unerase_all_erased_objects，通过遍历工作视图下被擦除的对象列表，然后逐个调用反擦除函数。
> 4. 主函数do_it，获取当前工作视图，然后调用unerase_all_erased_objects函数来反擦除所有被擦除的对象。
> 5. ufusr是UF程序入口函数，先调用UF_initialize初始化，然后调用do_it执行功能，最后调用UF_terminate终止。
> 6. ufusr_ask_unload用于指示程序应该立即卸载。
>
> 整体来看，这段代码实现了通过UF API遍历工作视图下所有被擦除的对象并反擦除的功能。代码结构清晰，主要功能通过自定义函数实现，具有较好的可读性和可维护性。
>
