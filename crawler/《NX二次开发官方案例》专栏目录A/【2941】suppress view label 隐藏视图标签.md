### 【2941】suppress view label 隐藏视图标签

#### 代码

```cpp
    /*HEAD SUPPRESS_VIEW_LABEL CCC UFUN */  
    /* This suppresses view labels for detail and section views 里海译:隐藏详细视图和节标题的标签 */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是 V18 中的新功能，用于打印系统日志信息。 */  
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
        char  
            msg[133];  
        while(( view = select_drawing_member_view("Select Section or Detail View")) != NULL_TAG)  
        {  
            UF_CALL(UF_DRAW_ask_view_label (view, &view_label));  
            UF_CALL(UF_DRAW_ask_view_label_parms(view_label, &view_label_parms));  
            view_label_parms.view_label = FALSE;  
            view_label_parms.scale_label = FALSE;  
            /* report view type, if desired: 里海译:报告视图类型 */  
            switch(view_label_parms.view_label_parm_type)  
            {  
                case UF_DRAW_view_label_other_view:  
                    strcpy(msg , "View type: other\n");  
                    break;  
                case UF_DRAW_view_label_detail_view:  
                    strcpy(msg , "View type: detail\n");  
                    break;  
                case UF_DRAW_view_label_section_view:  
                    strcpy(msg , "View type: section\n");  
                    break;  
            }  
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

> 这段代码是一段用于NX（NX是Siemens PLM Software公司的一款CAD/CAM/CAE软件）的二次开发代码，主要功能是隐藏详细视图和节标题的标签。具体来说，这段代码实现了以下几个功能：
>
> 1. 包含必要的头文件，包括UF、UF_UI、UF_DRAW等。
> 2. 定义了report_error函数，用于在函数调用出错时打印错误信息。
> 3. 定义了select_drawing_member_view函数，用于提示用户选择一个视图，并返回视图的标签。
> 4. 定义了do_it函数，是主逻辑函数。它循环调用select_drawing_member_view选择视图，然后获取视图的标签，并设置标签的显示参数为不显示视图标签和比例尺标签，从而实现隐藏视图标签的功能。
> 5. 定义了ufusr函数，是NX二次开发程序的入口函数。在初始化后调用do_it函数执行主逻辑，并在结束时进行终止。
> 6. 定义了ufusr_ask_unload函数，返回立即卸载，表示二次开发程序不需要驻留内存。
>
> 总体来说，这段代码通过获取视图标签并设置其显示参数，实现了隐藏详细视图和节标题标签的功能。代码结构清晰，注释详细，易于理解和使用。
>
