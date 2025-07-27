### 【2746】set current modeling view to fully shaded display 设置当前建模视图为全阴影显示

#### 代码

```cpp
    /*HEAD SET_CURRENT_MODELING_VIEW_TO_FULLY_SHADED_DISPLAY CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中新增的函数。翻译如下：

UF_print_syslog是V18版本中新增的函数。 */  
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
            view_data.display_mode = UF_VIEW_FULLY_SHADED;  
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

> 这段代码是一个NX Open API的二次开发示例，用于将当前工作视图设置为全阴影显示模式。主要功能包括：
>
> 1. 包含必要的头文件。
> 2. 定义了一个错误报告函数report_error，用于在API调用出错时打印错误信息。
> 3. 定义了一个主函数do_it，用于实现设置视图显示模式的功能。
> 4. 在do_it函数中，首先获取当前的工作视图，然后判断视图类型是否为建模视图。
> 5. 如果是建模视图，则获取视图的显示数据，并将其设置为全阴影模式，然后更新视图。
> 6. 如果不是建模视图，则打印提示信息。
> 7. ufusr函数是NX Open API的入口函数，调用UF_initialize初始化API，调用do_it实现功能，最后调用UF_terminate终止API。
> 8. ufusr_ask_unload函数用于卸载二次开发，这里设置为立即卸载。
>
> 总体来说，这段代码实现了将当前工作视图设置为全阴影显示模式的功能，代码结构清晰，注释详细，是一个很好的NX Open API二次开发示例。
>
