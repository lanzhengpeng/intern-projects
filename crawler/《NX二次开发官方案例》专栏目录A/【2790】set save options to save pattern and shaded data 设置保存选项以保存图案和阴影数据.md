### 【2790】set save options to save pattern and shaded data 设置保存选项以保存图案和阴影数据

#### 代码

```cpp
    /*HEAD SET_SAVE_OPTIONS_TO_SAVE_PATTERN_AND_SHADED_DATA CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_patt.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的函数。 */  
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
        UF_CALL( UF_PATT_set_archiving( UF_PATT_on ));  
        UF_CALL( UF_PATT_set_shading( UF_PATT_on ));  
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

> 这段代码是一个NX的二次开发代码，主要功能是设置NX的pattern和shaded数据保存选项。代码主要包含以下几个部分：
>
> 1. 错误报告函数：report_error函数用于报告函数调用失败的错误，它接收文件名、行号、调用字符串和错误码作为参数，并格式化错误消息输出到系统日志或NX的listing窗口中。
> 2. 设置pattern和shaded数据保存：do_it函数调用了UF_PATT_set_archiving和UF_PATT_set_shading两个函数，分别用于设置NX在保存文件时是否保存pattern数据和shaded数据。
> 3. NX初始化和终止：在ufusr函数中，首先调用UF_initialize进行NX初始化，然后调用do_it函数设置pattern和shaded数据的保存选项，最后调用UF_terminate终止NX。
> 4. 卸载提示：ufusr_ask_unload函数返回UF_UNLOAD_IMMEDIATELY，表示二次开发应用卸载后不需要等待NX重新启动。
>
> 总的来说，这段代码的主要目的是设置NX的pattern和shaded数据保存选项，以在保存文件时保留这些数据。同时，它也包含了错误报告和NX初始化/终止的相关代码。
>
