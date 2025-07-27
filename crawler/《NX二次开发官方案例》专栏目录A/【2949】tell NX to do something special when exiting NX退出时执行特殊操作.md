### 【2949】tell NX to do something special when exiting NX退出时执行特殊操作

#### 代码

```cpp
    /*  
        ufusr_ask_unload is invoked immediately following the completion of ufusr  
        (or ufsta when the dll is run from a startup folder) to find out when you  
        want the dll to be unloaded.  If ufusr_ask_unload returns  
        UF_UNLOAD_UG_TERMINATE, the ufusr_cleanup is called right before the dll is  
        unload when NX is terminating.  To prove to yourself that this works, set  
        the environment variable UGII_KEEP_SYSTEM_LOG=1 so that the syslog will not  
        automatically be cleaned up when NX terminates.  Start NX.  Use Help-> NX  
        Log File and make a note of the full path to the syslog which will be shown  
        as its own first line.  Run the dll built from this code (nothing noticable  
        will happen.)  Exit NX.  Find the syslog and open it in any text editor to  
        see this line near the bottom:  
    NX is shutting down - running ufusr_cleanup  
    */  
    #include <stdio.h>  
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
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
    }  
    /* qq3123197280 */  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_UG_TERMINATE);  
    }  
    void ufusr_cleanup(void)  
    {  
        ECHO("NX is shutting down - running ufusr_cleanup\n");  
        /* put your code here 里海译:"put your code here"的中文翻译是“在这里放置你的代码”。 */  
    }

```

#### 代码解析

> 这段代码是NX Open C API的一个示例，主要用于NX的二次开发。其中，ufusr和ufsta是NX调用的主函数，用于执行NX的启动和退出。ufusr_ask_unload函数用于在NX退出时决定是否卸载该动态链接库。如果返回UF_UNLOAD_UG_TERMINATE，则会在NX退出前调用ufusr_cleanup函数。ufusr_cleanup函数可以在其中添加需要清理的资源或执行其他退出前操作。此外，还定义了ECHO和report_error两个辅助函数，分别用于打印信息和报告错误。总体来说，这是一个NX二次开发的简单示例，实现了在NX启动和退出时执行特定操作的功能。
>
