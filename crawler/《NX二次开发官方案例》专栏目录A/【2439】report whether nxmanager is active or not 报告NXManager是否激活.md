### 【2439】report whether nxmanager is active or not 报告NXManager是否激活

#### 代码

```cpp
    /*HEAD REPORT_WHETHER_UGMANAGER_IS_ACTIVE_OR_NOT CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        logical  
            nxmanager_mode;  
        UF_CALL(UF_is_ugmanager_active(&nxmanager_mode));  
        WRITE_L(nxmanager_mode);  
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
    /*  To run in external NXManager use a command line argument: -pim=yes 里海译:在命令行中使用参数 -pim=yes 来在外部 NXManager 中运行。 */  
    int main( int argc, char *argv[] )  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这是NX的二次开发代码，主要功能是检查NX Manager是否处于激活状态。具体来说：
>
> 1. 包含了必要的NX开发头文件，并定义了一个错误报告函数report_error，用于在出错时打印错误信息。
> 2. 定义了一个写逻辑值到列表窗口的函数write_logical_to_listing_window。
> 3. 定义了一个函数do_it，该函数调用UF_is_ugmanager_active函数检查NX Manager是否激活，并通过write_logical_to_listing_window函数将结果写入列表窗口。
> 4. ufusr函数是NX的回调函数，用于初始化NX环境，执行do_it函数，然后终止NX环境。
> 5. ufusr_ask_unload函数用于卸载用户自定义模块。
> 6. 主函数main用于在NX Manager外部运行时初始化NX环境，执行do_it函数，然后终止NX环境。
> 7. 注释中提到了如果要在外部NX Manager中运行，需要在命令行中使用参数-pim=yes。
>
> 总体来说，这段代码的主要功能是检查NX Manager是否激活，并通过列表窗口显示结果，提供了在NX内部和外部两种方式运行的功能。
>
