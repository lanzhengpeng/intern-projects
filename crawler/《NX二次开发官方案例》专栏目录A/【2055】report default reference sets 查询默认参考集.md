### 【2055】report default reference sets 查询默认参考集

#### 代码

```cpp
    /*HEAD REPORT_DEFAULT_REFERENCE_SETS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 新增的功能。 */  
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
            ii,  
            n_ref_sets;  
        char  
            msg[MAX_LINE_SIZE+1],  
            **default_ref_sets;  
        UF_CALL(UF_ASSEM_ask_default_ref_sets(&n_ref_sets, &default_ref_sets));  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", "Number of Default Ref Sets", n_ref_sets);  
        UF_CALL(UF_UI_write_listing_window(msg));  
        for(ii=0; ii< n_ref_sets; ii++)  
        {  
            sprintf(msg, "\t%s\n", default_ref_sets[ii]);  
            UF_CALL(UF_UI_write_listing_window(msg));  
        }  
        UF_free_string_array(n_ref_sets, default_ref_sets);  
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

> 这段代码是一个NX Open C++应用程序，主要用于实现以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在UF函数调用出错时输出错误信息。
> 2. 定义了一个do_it函数，用于获取NX系统默认参考集的名称并输出。
> 3. 在ufusr函数中，首先初始化NX Open环境，然后调用do_it函数，最后终止NX Open环境。
> 4. 定义了一个ufusr_ask_unload函数，返回立即卸载标志，表示在应用程序退出后立即卸载。
>
> 该应用程序实现了获取NX系统默认参考集名称并输出的功能，使用NX Open C++ API进行二次开发。通过定义错误报告函数，可以方便地处理UF函数调用错误。同时，实现了在应用程序退出后立即卸载NX Open环境。
>
