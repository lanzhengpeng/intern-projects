### 【2950】test api execution UF_CALL(UF_is_ugmanager_active(&nxmanager_mode))；

#### 代码

```cpp
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_exit.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        ECHO(msg);  
    }  
    static void do_it(void)  
    {  
        logical  
            nxmanager_mode;  
        UF_CALL(UF_is_ugmanager_active(&nxmanager_mode));  
        WRITE_L(nxmanager_mode);  
    }  
    /* qq3123197280 */  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
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
    /*  To run in external NXManager use a command line argument: -pim=yes 里海译:要在外部NXManager中运行，请使用命令行参数：-pim=yes */  
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

> 这段代码是NX Open C++ API编写的NX二次开发代码，其主要功能是检测NX Manager是否处于活动状态。
>
> 关键点包括：
>
> 1. 包含NX Open C++ API的头文件，如uf.h、uf_ui.h等，用于调用NX提供的API函数。
> 2. 定义了一个宏UF_CALL，用于调用NX API函数，并报告错误。如果API调用失败，会打印错误信息。
> 3. 定义了一个宏WRITE_L，用于将逻辑值输出到列表窗口。
> 4. do_it函数中，调用UF_is_ugmanager_active函数检测NX Manager是否处于活动状态，并通过WRITE_L输出结果。
> 5. ufsta和ufusr是NX二次开发插件必须实现的两个函数，分别用于NX启动和关闭时调用。这里在启动时调用do_it函数。
> 6. ufusr_ask_unload函数用于卸载插件时的处理，这里返回立即卸载。
> 7. main函数用于程序的入口，初始化NX环境，调用uc4624函数(用于处理命令行参数)，调用do_it函数，然后终止NX环境。
> 8. 注释表明要在外部NX Manager中运行，需要在命令行中添加参数"-pim=yes"。
>
> 综上所述，这段代码的主要功能是检测NX Manager是否激活，并在NX启动时输出检测结果。它使用了NX Open C++ API来实现NX的二次开发。
>
