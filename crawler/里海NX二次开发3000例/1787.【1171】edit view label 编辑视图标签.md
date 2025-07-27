### 【1171】edit view label 编辑视图标签

#### 代码

```cpp
    /*HEAD EDIT_VIEW_LABEL CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
    #include <uf_draw_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是 V18 版本中新增的函数。 */  
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
            view_label,  
            view;  
        UF_DRAW_view_label_parms_t   
            view_label_parms;  
        while(( view = select_drawing_member_view("Select Section or Detail View")) != NULL_TAG)  
        {  
            UF_CALL(UF_DRAW_ask_view_label (view, &view_label));  
            UF_CALL(UF_DRAW_ask_view_label_parms(view_label, &view_label_parms));  
            strcpy(view_label_parms.view_letter, "A");  
            UF_DRAW_set_view_label_parms(view_label, &view_label_parms);  
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

> 这段代码是一个NX Open C++二次开发示例，主要实现以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在发生UF函数调用错误时，打印错误信息到系统日志和列表窗口。
> 2. 定义了一个选择绘图成员视图的函数select_drawing_member_view，通过用户提示选择一个截面视图或细节视图。
> 3. 定义了一个执行主体函数do_it，用于循环选择视图，获取视图标签，设置视图标签参数，并修改视图标签字母为"A"。
> 4. 定义了ufusr函数，作为程序的入口函数，首先初始化NX Open，然后调用do_it执行主体功能，最后终止NX Open。
> 5. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示程序不需要在NX中保留状态信息。
>
> 这段代码的主要功能是选择图纸中的视图，修改其视图标签字母为"A"，以实现视图的标准化管理。代码结构清晰，提供了错误处理和用户交互，是一个典型的NX二次开发示例。
>
