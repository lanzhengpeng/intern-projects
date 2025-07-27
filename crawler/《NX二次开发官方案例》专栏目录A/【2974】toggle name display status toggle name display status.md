### 【2974】toggle name display status toggle name display status

#### 代码

```cpp
    /*HEAD TOGGLE_NAME_DISPLAY_STATUS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中的新功能。

只回答译文，不要废话。 */  
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
    static void do_it(void)  
    {  
        int  
            display_status,  
            view_status;  
        tag_t  
            work_view;  
        UF_CALL(UF_DISP_ask_name_view_status(&view_status));  
        if (view_status == UF_DISP_NAMES_IN_VIEW_OF_DEFN)  
            UF_CALL(UF_DISP_set_name_view_status(UF_DISP_NAMES_IN_WORK_VIEW));  
        UF_CALL(UF_DISP_ask_name_display_status(&display_status));  
        UF_CALL(UF_DISP_set_name_display_status(1 - display_status));  
    /*  this should not be necessary - see PR 4339206 里海译:PR 4339206 中提到了，这个应该是不必要的。 */  
        UF_CALL(UF_VIEW_ask_work_view(&work_view));  
        UF_CALL(UF_DISP_regenerate_view(work_view));  
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

> 这段代码是NX二次开发代码，其主要功能是实现模型树中显示名称的切换。具体来说，代码首先获取当前模型树名称的显示状态，然后将其设置为相反的状态。具体步骤包括：
>
> 1. 使用UF_DISP_ask_name_view_status函数获取当前模型树名称的显示状态，保存在view_status变量中。
> 2. 如果当前状态为UF_DISP_NAMES_IN_VIEW_OF_DEFN，则使用UF_DISP_set_name_view_status函数将其设置为UF_DISP_NAMES_IN_WORK_VIEW。
> 3. 使用UF_DISP_ask_name_display_status函数获取当前名称是否显示，保存在display_status变量中。
> 4. 使用UF_DISP_set_name_display_status函数将名称显示状态设置为相反状态。
> 5. 使用UF_VIEW_ask_work_view函数获取当前工作视图。
> 6. 使用UF_DISP_regenerate_view函数刷新当前工作视图。
> 7. ufusr函数首先初始化NX系统，然后调用do_it函数执行上述功能，最后终止NX系统。
> 8. ufusr_ask_unload函数返回UF_UNLOAD_IMMEDIATELY，表示不需要卸载。
>
> 通过这段代码，用户可以方便地切换模型树中名称的显示状态，实现名称的显示与隐藏。
>
