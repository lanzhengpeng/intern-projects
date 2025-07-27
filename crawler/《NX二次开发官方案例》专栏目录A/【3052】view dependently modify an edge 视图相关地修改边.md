### 【3052】view dependently modify an edge 视图相关地修改边

#### 代码

```cpp
    /*HEAD VIEW_DEPENDENTLY_MODIFY_AN_EDGE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文中的描述，UF_print_syslog是V18版本中新增的功能。 */  
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
    static tag_t select_an_edge(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            irc;  
        tag_t  
            edge,  
            view;  
        UF_UI_selection_options_t  
            opts;  
        UF_UI_mask_t  
            mask;  
        opts.num_mask_triples = 1;  
        opts.mask_triples = &mask;  
        opts.scope = UF_UI_SEL_SCOPE_WORK_PART;  
        mask.object_type = UF_solid_type;  
        mask.object_subtype = 0;  
        mask.solid_type = UF_UI_SEL_FEATURE_ANY_EDGE;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_single(prompt, &opts, &irc, &edge, cp, &view));  
        if (irc == 4 || irc == 5)  
        {  
            UF_CALL(UF_DISP_set_highlight(edge, FALSE));  
            return edge;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        char    view_name[50];  
        tag_t   edge;  
        double  parms[2] = { 0.2, 0.8};  
        int     any_view = 0, color = -1, font = 0, width = -1;  
        /* Set view of cursor to any 里海译:将光标视图设置为任意。 */  
        UF_CALL(UF_UI_set_cursor_view( any_view ));  
        while( (edge = select_an_edge("Select edge to modify"))   
            && (edge != NULL_TAG))  
        {  
            /* Return view name of last position indication 里海译:返回最后位置指示的视图名称 */  
            UF_CALL( uc1653( view_name ));  
            UF_CALL( uc6406 ( view_name, edge, parms, color, font,  width));  
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

> 这段代码是用于NX的二次开发，其功能是允许用户选择一个边并修改其显示属性。主要特点如下：
>
> 1. 函数介绍：代码包含了几个主要函数，包括main函数ufusr，用于初始化和调用主功能函数do_it。do_it函数用于循环选择边并修改其显示属性。另外还包括select_an_edge函数，用于选择边，以及report_error函数，用于错误报告。
> 2. 错误报告：report_error函数用于捕捉函数调用错误，并打印错误信息。它通过UF_get_fail_message获取错误描述，并通过UF_print_syslog打印到系统日志，同时也可以打印到NX的Listing窗口。
> 3. 选择边：select_an_edge函数用于选择一个边，它通过UF_UI_set_cursor_view设置光标视图，并通过UF_UI_select_single让用户选择边。
> 4. 修改显示属性：do_it函数循环调用select_an_edge，获取用户选择的边，然后调用uc1653和uc6406两个函数来修改边的显示属性，如颜色、字体、线宽等。
> 5. NX API调用：代码中使用了大量NX提供的API函数，如UF_DISP_set_highlight、UF_UI_set_cursor_view、UF_UI_select_single等，用于实现交互式选择和修改边的显示属性。
> 6. NX版本兼容：代码中使用了V18版本中新增的UF_print_syslog函数，表明这段代码适用于NX V18及以上版本。
> 7. 注释说明：代码中包含了中文注释，用于解释函数的作用以及一些关键步骤。
>
> 总体而言，这段代码实现了在NX中允许用户选择边并修改其显示属性的功能，是一个典型的NX二次开发示例。
>
