### 【2164】report is type displayable 报告是否为可显示类型

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
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
    static int prompt_for_an_integer(char *prompt, char *item, int number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        int  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            return da[0];  
        }  
        else return 0;  
    }  
    #define WRITE_L(X) if (X) ECHO("%s = TRUE\n", #X); \  
        else ECHO("%s = FALSE\n", #X);  
    // Note UF_OBJ_is_type_displayable is replaced by UF_OBJ_is_displayable  
    static void do_it(void)  
    {  
        int  
            n = 0;  
        while ((n = prompt_for_an_integer("Displayable object?", "Type", n)) != 0)  
            WRITE_L(UF_OBJ_is_type_displayable(n));  
    }  
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

> 这段代码是NX二次开发代码，主要实现了以下功能：
>
> 1. 包含了必要的NX二次开发头文件，如uf.h、uf_ui.h、uf_obj.h等。
> 2. 定义了一个ECHO宏，用于打印信息到日志窗口和系统日志。
> 3. 定义了一个UF_CALL宏，用于调用NX API函数并报告错误。
> 4. 实现了一个report_error函数，用于打印函数调用失败时的错误信息。
> 5. 实现了一个prompt_for_an_integer函数，用于弹出一个对话框让用户输入一个整数。
> 6. 定义了一个WRITE_L宏，用于打印变量值。
> 7. 实现了一个do_it函数，循环调用prompt_for_an_integer函数获取用户输入的整数，并打印该整数类型是否可显示。
> 8. 实现了ufusr函数，这是NX二次开发程序的入口函数，在这里调用do_it函数。
> 9. 实现了ufusr_ask_unload函数，用于卸载二次开发程序。
>
> 总的来说，这段代码实现了让用户输入一个整数，然后打印该整数类型是否可显示的功能。使用了NX的对话框API来实现输入，以及对象类型API来判断是否可显示。
>
