### 【2971】toggle line width display 切换线宽显示

#### 代码

```cpp
    /*HEAD TOGGLE_LINE_WIDTH_DISPLAY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是在V18版本中新增的函数。因此，对于“UF_print_syslog是V18新增的”这个问题，我的回答是：UF_print_syslog是V18新增的。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        UF_DISP_system_params_t  
            system_parameters;  
        UF_CALL(UF_DISP_ask_system_parameters(&system_parameters));  
        WRITE("Old: ");  
        WRITE_D(system_parameters.line_width_display);  
        system_parameters.line_width_display =  
            3 - system_parameters.line_width_display;  
        WRITE("New: ");  
        WRITE_D(system_parameters.line_width_display);  
        UF_CALL(UF_DISP_set_system_parameters(&system_parameters));  
        UF_CALL(UF_DISP_regenerate_display());  /* To see effect of toggle! 里海译:To see effect of toggle!的翻译是“查看切换效果！”。 */  
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

> 根据代码内容，这是一段用于切换NX线宽显示的二次开发代码，主要功能包括：
>
> 1. 引入头文件：代码引入了必要的NX Open头文件，包括uf.h、uf_ui.h、uf_disp.h等，用于调用NX提供的二次开发接口。
> 2. 错误报告函数：定义了report_error函数，用于在调用NX接口失败时，输出错误信息，包括错误码、错误信息、出错的文件名和行号等。
> 3. 整数写入函数：定义了write_integer_to_listing_window函数，用于将一个整数写入列表窗口，便于调试。
> 4. 切换线宽实现：在do_it函数中，首先获取当前的系统参数，然后切换线宽显示的参数值（从0改为1，或从1改为0），最后设置新的系统参数并重新生成显示，以看到切换效果。
> 5. ufusr函数：这是NX二次开发的标准入口函数，首先初始化NX环境，然后调用do_it函数执行切换线宽的操作，最后终止NX环境。
> 6. 卸载函数：定义了ufusr_ask_unload函数，返回立即卸载，表示当关闭NX时，该二次开发模块会被立即卸载。
>
> 总的来说，这段代码通过调用NX的二次开发接口，实现了简单的切换线宽显示的功能，并通过错误报告和写入窗口等操作，便于调试和查看运行结果。
>
