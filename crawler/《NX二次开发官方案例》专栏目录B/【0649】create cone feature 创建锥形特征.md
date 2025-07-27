### 【0649】create cone feature 创建锥形特征

#### 代码

```cpp
    /*HEAD CREATE_CONE_FEATURE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog 是 V18 中新增的功能，用于打印系统日志信息。 */  
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
            cone;  
        double  
            direction[3] = { 1, 0, 0 },  
            origin[3] = { 0, 0, 0 };  
        char  
            *dia[2] = { "2.0", "1.0"};  
        UF_CALL(UF_MODL_create_cone1(UF_NULLSIGN, origin, "1.0", dia,  
            direction, &cone));  
    }  
    /* 里海 */  
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

> 这段代码是NX的二次开发示例代码，用于创建圆锥特征。主要包括以下部分：
>
> 1. 包含了必要的NX头文件。
> 2. 定义了report_error函数，用于在发生错误时报告错误信息。
> 3. 定义了do_it函数，用于创建一个圆锥特征。该函数指定了圆锥的顶点、底面直径、高度和方向等参数。
> 4. ufusr函数是NX二次开发的标准入口函数，在这里调用了do_it函数，并初始化/终止NX环境。
> 5. ufusr_ask_unload函数用于定义当模块被卸载时的行为，这里设置为立即卸载。
> 6. 使用了UF_CALL宏来捕获函数调用是否成功，并在失败时调用report_error函数。
> 7. 通过参数指定了圆锥的顶点、底面直径、高度和方向等信息。
>
> 总的来说，这段代码实现了创建圆锥特征的功能，并包含了错误处理和模块管理的基本框架，为NX二次开发提供了一个参考示例。
>
