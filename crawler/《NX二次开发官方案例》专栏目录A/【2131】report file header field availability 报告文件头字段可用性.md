### 【2131】report file header field availability 报告文件头字段可用性

#### 代码

```cpp
    /*HEAD REPORT_FILE_HEADER_FIELD_AVAILABILITY CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #define IS_BIT_ON(X,Y) ((X) & (0x01 << (Y)))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能。 */  
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
            qreslt;  
        UF_CALL(uc4596(2, &qreslt));  
        UF_UI_open_listing_window();  
        if (IS_BIT_ON(qreslt, 0))  
            UF_UI_write_listing_window("Owner is supported\n");  
        else  
            UF_UI_write_listing_window("Owner is NOT supported\n");  
        if (IS_BIT_ON(qreslt, 1))  
            UF_UI_write_listing_window("Protection Classes supported\n");  
        else  
            UF_UI_write_listing_window("Protection Classes NOT supported\n");  
        if (IS_BIT_ON(qreslt, 2))  
            UF_UI_write_listing_window("Status supported\n");  
        else  
            UF_UI_write_listing_window("Status NOT supported\n");  
        if (IS_BIT_ON(qreslt, 5))  
            UF_UI_write_listing_window("Description supported\n");  
        else  
            UF_UI_write_listing_window("Status NOT supported\n");  
        if (IS_BIT_ON(qreslt, 6))  
            UF_UI_write_listing_window("Customer Area supported\n");  
        else  
            UF_UI_write_listing_window("Customer Area NOT supported\n");  
        if (IS_BIT_ON(qreslt, 7))  
            UF_UI_write_listing_window("Non-Native Files supported\n");  
        else  
            UF_UI_write_listing_window("Non-Native Files NOT supported\n");  
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

> 这段NX二次开发代码主要用于查询NX系统中的文件信息功能支持情况，并输出到列表窗口。以下是代码的主要功能介绍：
>
> 1. 引入头文件：代码引入了标准C库、UF库、UF UI库和UF CFI库的头文件，以支持后续代码的运行。
> 2. 错误处理宏：定义了错误处理宏IS_BIT_ON和UF_CALL，用于判断位状态和调用UF函数时的错误处理。
> 3. 错误报告函数：定义了report_error函数，用于在UF函数调用出错时报告错误信息。
> 4. 功能实现函数：定义了do_it函数，用于查询NX系统对文件信息功能的各项支持情况，并输出到列表窗口。
> 5. UF初始化和终止：在ufusr函数中，首先调用UF_initialize初始化UF环境，然后调用do_it函数实现主要功能，最后调用UF_terminate终止UF环境。
> 6. 卸载询问：定义了ufusr_ask_unload函数，用于在卸载时询问是否立即卸载。
>
> 总体来看，这段代码实现了对NX系统文件信息功能的查询和输出功能，采用了UF库进行二次开发。代码结构清晰，功能明确。
>
