### 【0522】convert parts list to model mode 转换零件清单为模型模式

#### 代码

```cpp
    /*HEAD CONVERT_PARTS_LIST_TO_MODEL_MODE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_undo.h>  
    #include <uf_plist.h>  
    #include <uf_tabnot.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog 是 V18 的新功能。 */  
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
    static void convert_to_model(tag_t object)  
    {  
        int  
            status;  
        tag_t  
            view_tag;  
        char  
            view_name[MAX_ENTITY_NAME_SIZE];  
        if (!uc6409(object, &status, view_name) && (status == 1))  
        {  
        /*  Try the new call first, if that fails - try the old call too 译:尝试新的调用，如果失败，再尝试旧的调用。 */  
            UF_CALL(UF_VIEW_ask_tag_of_view_name(view_name, &view_tag));  
            if (UF_CALL(UF_VIEW_convert_to_model(view_tag, object)))  
            {  
                UF_UI_write_listing_window("Using uc6401 instead...\n");  
                UF_CALL(uc6401(view_name, object));  
                UF_UI_write_listing_window("\n");  
            }  
        }  
    }  
    #include <uf_undo.h>  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            plist,  
            plist_section;  
        plist = UF_PLIST_ask_tag();  
        if (plist != NULL_TAG)  
        {  
            set_undo_mark("Convert Parts List to Model Mode");  
            UF_CALL(UF_TABNOT_ask_nm_sections(plist, &n));  
            for (ii = 0; ii < n; ii++)  
            {  
                UF_CALL(UF_TABNOT_ask_nth_section(plist, ii, &plist_section));  
                convert_to_model(plist_section);  
            }  
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

> 这段NX二次开发代码的主要功能是将NX中的部件清单(Part List)转换为模型模式(Model Mode)。
>
> 具体步骤如下：
>
> 1. 定义了一个错误报告函数report_error，用于输出函数调用失败时的错误码和错误信息。
> 2. 定义了convert_to_model函数，该函数接收一个部件清单段落的标签，首先尝试使用新的UF_VIEW接口将段落转换为模型模式，如果失败则尝试使用旧的uc6401接口。
> 3. 定义了set_undo_mark函数，用于设置撤销标记，以便后续撤销操作。
> 4. 定义了do_it函数，该函数首先获取当前部件清单的标签，然后获取部件清单中的所有段落，并调用convert_to_model函数将每个段落转换为模型模式。
> 5. ufusr函数是二次开发程序的入口函数，首先初始化NX环境，然后调用do_it函数执行转换操作，最后终止NX环境。
> 6. ufusr_ask_unload函数返回立即卸载，表示二次开发程序不需要在NX中保留状态信息。
>
> 总的来说，这段代码的主要功能是遍历部件清单中的所有段落，并将它们转换为模型模式。同时，代码中包含了错误处理和撤销功能。
>
