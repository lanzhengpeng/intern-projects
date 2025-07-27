### 【2814】set view visualization 设置视图可视化

#### 代码

```cpp
    /*HEAD SET_VIEW_VISUALIZATION CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog是在V18版本中新增的函数，用于打印系统日志。 */  
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
            view_data.hidden_edge_mode = UF_VIEW_HIDDEN_EDGES_DASHED;  
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

> 这段代码是NX的二次开发代码，主要功能是设置工作视图的隐藏边显示方式为虚线。具体介绍如下：
>
> 1. 包含了必要的NX API头文件。
> 2. 定义了一个错误报告函数report_error，用于在调用NX API时检测并打印错误信息。
> 3. 定义了主函数do_it，其中：获取当前的工作视图tag。检查工作视图类型，如果是模型视图，则获取其可视化设置。将隐藏边的显示方式设置为虚线。应用新的可视化设置到工作视图。
> 4. 获取当前的工作视图tag。
> 5. 检查工作视图类型，如果是模型视图，则获取其可视化设置。
> 6. 将隐藏边的显示方式设置为虚线。
> 7. 应用新的可视化设置到工作视图。
> 8. 定义了ufusr函数，作为二次开发程序的入口，调用do_it函数。
> 9. 定义了ufusr_ask_unload函数，返回立即卸载，表示二次开发程序不需要在NX中保持加载状态。
>
> 总体来说，这段代码通过调用NX的视图API，实现了设置工作视图隐藏边显示方式的功能，为用户提供了一个便捷的视图定制选项。
>
