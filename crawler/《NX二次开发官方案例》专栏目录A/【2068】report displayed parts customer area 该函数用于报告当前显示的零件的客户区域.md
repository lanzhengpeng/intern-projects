### 【2068】report displayed parts customer area 该函数用于报告当前显示的零件的客户区域

#### 代码

```cpp
    /*HEAD REPORT_DISPLAYED_PARTS_CUSTOMER_AREA CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18中新增的函数，用于打印系统日志信息。 */  
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
        char  
            carea[132] = "",  
            pspec[MAX_FSPEC_SIZE + 1];  
        tag_t  
            part = UF_PART_ask_display_part();  
        UF_PART_ask_part_name(part, pspec);  
        UF_CALL(uc4564(pspec, 0, (1 << 6)));  
        uc4608(carea);  
        printf("Part: %s  has the following customer area:\n %s \n", pspec, carea);  
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

> 这段代码是用于NX的二次开发，其主要功能是获取当前显示部件的名称，并获取该部件的客户区域内容。
>
> 代码的主要逻辑如下：
>
> 1. 包含必要的NX头文件，如uf.h、uf_ui.h、uf_part.h等。
> 2. 定义了一个宏UF_CALL，用于记录函数调用信息。
> 3. 定义了一个错误报告函数report_error，用于输出函数调用失败的信息。
> 4. 定义了主函数do_it，用于获取当前显示部件的名称，并获取该部件的客户区域内容。
> 5. ufusr函数是NX二次开发的入口函数，调用do_it函数，并初始化和终止NX环境。
> 6. ufusr_ask_unload函数用于设置NX二次开发模块卸载方式。
>
> 总体来说，这段代码实现了简单的二次开发，能够获取当前显示部件的客户区域信息，并输出了相关的信息。
>
