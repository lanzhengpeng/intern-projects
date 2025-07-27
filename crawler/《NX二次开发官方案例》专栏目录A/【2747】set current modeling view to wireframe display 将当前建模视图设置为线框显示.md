### 【2747】set current modeling view to wireframe display 将当前建模视图设置为线框显示

#### 代码

```cpp
    /*HEAD SET_CURRENT_MODELING_VIEW_TO_WIREFRAME_DISPLAY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能，用于打印系统日志。 */  
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
        tag_t  
            work_view;  
        UF_VIEW_type_t  
            type;  
        UF_VIEW_subtype_t  
            subtype;  
        UF_VIEW_visualization_t  
            view_data;  
        UF_CALL(UF_VIEW_ask_work_view(&work_view));  
        UF_CALL(UF_VIEW_ask_type(work_view, &type, &subtype));  
        if (type == UF_VIEW_MODEL_TYPE)  
        {  
            UF_CALL(UF_VIEW_ask_visualization(work_view, &view_data));  
            view_data.display_mode = UF_VIEW_NOT_SHADED;  
            UF_CALL(UF_VIEW_set_visualization(work_view, &view_data));  
        }  
        else  
            uc1601("Work view is not a modeling view", TRUE);  
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

> 这段代码是NX的二次开发代码，主要实现了将当前工作视图设置为线框显示模式的功能。
>
> 主要步骤如下：
>
> 1. 定义了一个报告错误的函数report_error，用于在函数调用失败时输出错误信息。
> 2. 定义了主函数do_it，首先获取当前的工作视图，并查询其视图类型。
> 3. 如果是建模视图类型，则获取其可视化数据，并将显示模式设置为线框显示，然后更新视图。
> 4. 如果不是建模视图类型，则输出错误信息。
> 5. ufusr函数是NX调用的主函数，在这里初始化NX环境，调用do_it函数，然后终止NX环境。
> 6. ufusr_ask_unload函数用于设置二次开发库的卸载方式。
>
> 总的来说，这段代码实现了将当前工作视图设置为线框显示的功能，通过查询视图类型，设置不同的显示模式，实现了对视图显示方式的控制。
>
