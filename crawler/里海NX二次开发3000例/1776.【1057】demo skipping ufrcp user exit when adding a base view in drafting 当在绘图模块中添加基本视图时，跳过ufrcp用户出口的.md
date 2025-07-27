### 【1057】demo skipping ufrcp user exit when adding a base view in drafting 当在绘图模块中添加基本视图时，跳过ufrcp用户出口的

#### 代码

```cpp
    #include <stdarg.h>  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_exit.h>  
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
    // USER_RCOMP  
    void ufrcp(char *param, int *retcode, int paramLen)  
    {  
        int  
            application;  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_ask_application_module(&application));  
        if (application == UF_APP_DRAFTING)  
        {  
            UF_print_syslog("Skipping ufrcp User Exit for Add Base View\n", FALSE);  
            *retcode = 4;  
        }  
        else  
        {  
            UF_print_syslog("Running ufrcp User Exit for Add Component\n", FALSE);  
            strcpy(param, "Z:\\parts\\sample.prt");  
            *retcode = 3;  
        }  
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
> 1. 定义了ECHO宏，用于打印信息到NX的日志窗口和系统日志。
> 2. 定义了UF_CALL宏，用于调用NX的UF函数，并检查错误码。如果函数返回错误，会打印错误信息。
> 3. 实现了ufrcp函数，该函数根据当前NX的应用模块，如果是草绘模块，则返回错误码4；如果是组件模块，则将参数修改为指定的文件路径，并返回错误码3。
> 4. 实现了ufusr_ask_unload函数，返回立即卸载标志，表示该二次开发模块不需要在NX启动后一直加载。
> 5. 整体上，这段代码通过二次开发的方式，实现了对NX软件特定功能的自定义，根据不同的模块执行不同的逻辑，体现了二次开发对软件扩展性的重要作用。
>
