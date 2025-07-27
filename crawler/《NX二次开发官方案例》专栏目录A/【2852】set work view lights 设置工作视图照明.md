### 【2852】set work view lights 设置工作视图照明

#### 代码

```cpp
    /*HEAD SET_WORK_VIEW_LIGHTS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_view.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本新增的函数，用于打印系统日志信息。 */  
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
        UF_VIEW_lighting_t  
            view_light;  
        UF_CALL(UF_VIEW_ask_work_view(&work_view));  
        view_light.two_sided_light = 0;  
        view_light.shininess = 0.70;  
        view_light.light_count = 3;  
        strcpy(view_light.lights[0].light_name, "Default 1 Ambient");  
        view_light.lights[0].light_type = 0;  
        view_light.lights[0].light_mode = 1;  
        view_light.lights[0].location[0] = 0.0;  
        view_light.lights[0].location[1] = 0.0;  
        view_light.lights[0].location[2] = 1.0;  
        view_light.lights[0].to[0] = 0.0;  
        view_light.lights[0].to[1] = 0.0;  
        view_light.lights[0].to[2] = 0.0;  
        strcpy(view_light.lights[1].light_name, "Std Up-Left Dist");  
        view_light.lights[1].light_type = 1;  
        view_light.lights[1].light_mode = 0;  
        view_light.lights[1].location[0] = -12000.0;  
        view_light.lights[1].location[1] = 6000.0;  
        view_light.lights[1].location[2] = 12000.0;  
        view_light.lights[1].to[0] = 0.0;  
        view_light.lights[1].to[1] = 0.0;  
        view_light.lights[1].to[2] = 0.0;  
        strcpy(view_light.lights[2].light_name, "Std Up-Right Dist");  
        view_light.lights[2].light_type = 1;  
        view_light.lights[2].light_mode = 0;  
        view_light.lights[2].location[0] = 12000.0;  
        view_light.lights[2].location[1] = 12000.0;  
        view_light.lights[2].location[2] = 3000.0;  
        view_light.lights[2].to[0] = 0.0;  
        view_light.lights[2].to[1] = 0.0;  
        view_light.lights[2].to[2] = 0.0;  
        UF_CALL(UF_VIEW_set_view_light(work_view, &view_light));  
    /*  It is necessary to regenerate the display to see the effect of  
        applying the lights to the view.  See PR 5608349  
    */  
        UF_CALL(UF_DISP_regenerate_display());  
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

> 这段代码是一个NX的二次开发示例，其主要功能是设置工作视图的灯光。
>
> 代码的主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于打印函数调用失败时的错误信息。
> 2. 定义了一个设置灯光的函数do_it。首先获取当前的工作视图，然后定义了一个UF_VIEW_lighting结构，设置了两面光照、高光、灯光数量等参数，并设置了3个灯光的名称、类型、位置等属性，最后调用UF_VIEW_set_view_light函数将灯光应用到工作视图。
> 3. 在ufusr函数中，首先初始化NX，然后调用do_it函数设置灯光，最后终止NX。
> 4. 定义了一个卸载函数ufusr_ask_unload，表示立即卸载当前加载的二次开发程序。
>
> 这段代码展示了如何在NX中获取工作视图，设置灯光参数，并将灯光应用到视图上，以及如何处理函数调用失败的情况。
>
