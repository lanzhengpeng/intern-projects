### 【2753】set distance tolerance 设置距离容差

#### 代码

```cpp
    /*HEAD SET_DISTANCE_TOLERANCE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #define ECHOF(X)    (printf("%s = %f\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的函数，用于打印系统日志消息。

所以，UF_print_syslog在V18版本中是新增的。 */  
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
            err_flag;  
        double  
            original_tolerance,  
            tolerance = .001,  
            modified_tolerance;  
        UF_MODL_ask_distance_tolerance(&original_tolerance);  
        ECHOF(original_tolerance);  
        UF_MODL_set_distance_tolerance(&tolerance, &err_flag);  
        if (err_flag != 0) printf("Invalid value, tolerance not changed\n");  
        UF_MODL_ask_distance_tolerance(&modified_tolerance);  
        ECHOF(modified_tolerance);  
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

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 包含了标准头文件，用于调用NX的API函数。
> 2. 定义了一个错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 3. 定义了一个执行函数do_it，用于查询和修改NX的距离公差。
> 4. 在do_it函数中，首先使用UF_MODL_ask_distance_tolerance查询当前的公差值并打印。
> 5. 然后使用UF_MODL_set_distance_tolerance设置新的公差值，并检查设置是否成功。
> 6. 最后再次查询公差值并打印，以验证设置是否生效。
> 7. 定义了ufusr函数作为NX的回调函数，在NX启动时调用do_it函数。
> 8. 定义了ufusr_ask_unload函数，返回立即卸载标志，用于卸载二次开发代码。
> 9. 整个代码流程清晰，实现了查询和设置NX的距离公差的功能，并具有错误处理机制。
>
> 以上是对这段NX二次开发代码的主要功能的简要介绍。
>
