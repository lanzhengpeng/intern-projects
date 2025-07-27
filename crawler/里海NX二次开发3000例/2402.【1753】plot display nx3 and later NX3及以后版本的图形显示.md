### 【1753】plot display nx3 and later NX3及以后版本的图形显示

#### 代码

```cpp
    /*HEAD PLOT_DISPLAY_NX3_AND_LATER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_plot.h>  
    #include <uf_part.h>  
    #include <uf_ugmgr.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog是V18版本中新增的功能。它用于打印系统日志。 */  
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
            units;  
        tag_t  
            part = UF_PART_ask_display_part();  
        UF_PLOT_rotation_t   
            rotation = UF_PLOT_ROTATION_0;  
        double  
            scale = 1.0,  
            origin[2] = { 0.0,  0.0};  
        UF_CALL(UF_PART_ask_units(part, &units));  
    /*  for actual use, you will probably want to have argument's   
            that are more pertinent to your requirements */  
        UF_CALL(UF_PLOT_add_job_to_plot_layout(NULL_TAG, NULL, "test", units,  
             origin, rotation, scale, NULL));  
        UF_CALL(UF_PLOT_print_plot_layout( "test", NULL, NULL, NULL, 1));  
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

> 这段代码是用于NX二次开发的UFUNC功能，主要实现了以下功能：
>
> 1. 包含了必要的NX API头文件。
> 2. 定义了错误处理函数report_error，用于在函数调用失败时打印错误信息。
> 3. 定义了主函数do_it，用于执行以下操作：获取当前显示的零件。获取零件的长度单位。创建一个名为"test"的绘图布局，设置原点、旋转角度和缩放比例。添加当前零件到绘图布局中。打印输出该绘图布局。
> 4. 获取当前显示的零件。
> 5. 获取零件的长度单位。
> 6. 创建一个名为"test"的绘图布局，设置原点、旋转角度和缩放比例。
> 7. 添加当前零件到绘图布局中。
> 8. 打印输出该绘图布局。
> 9. 定义了ufusr函数，用于在NX启动时调用do_it函数。
> 10. 定义了ufusr_ask_unload函数，用于在卸载该UFUNC时立即卸载。
>
> 总体来说，这段代码实现了在NX中创建一个绘图布局并打印输出的功能。
>
