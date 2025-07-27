### 【3102】demo specify plane 使用NX API指定平面

#### 代码

```cpp
    /*HEAD DEMO_SPECIFY_PLANE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    static logical ask_yes_or_no(char *title, char *message)  
    {  
        int  
            resp;  
        UF_UI_message_buttons_t  
            buttons = { TRUE, FALSE, TRUE, "Yes", NULL, "No", 1, 0, 2 };  
        UF_CALL(UF_UI_message_dialog(title, UF_UI_MESSAGE_QUESTION, &message, 1,  
            TRUE, &buttons, &resp));  
        return (2 - resp);  
    }  
    static void do_it(void)  
    {  
        int  
            display,  
            mode = UF_UI_NO_DEFAULT,  
            resp;  
        tag_t  
            plane;  
        double  
            orientation[9],  
            point[3];  
    /*  return is:  1 = yes, 0 = no 里海译:根据上下文，翻译 "return is: 1 = yes, 0 = no" 的意思是“返回值是：1表示是，0表示否”。 */  
        display = ask_yes_or_no("Specify Plane", "Display temporary plane");  
    /*  input needs: 0 = yes, 1 = no 里海译:0 = 是，1 = 否 */  
        display = 1 - display;  
        UF_CALL(UF_UI_specify_plane("Specify Plane", &mode, display, &resp,  
            orientation, point, &plane));  
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

> 这段代码是NX Open C++ API编写的NX二次开发示例代码，用于实现用户指定平面的功能。
>
> 主要功能包括：
>
> 1. 定义了错误报告函数report_error，用于在函数执行出错时打印错误信息。
> 2. 定义了询问函数ask_yes_or_no，用于询问用户是否显示临时平面。
> 3. 定义了主函数do_it，用于实现指定平面的功能。
> 4. 在ufusr函数中，初始化并调用do_it函数，最后终止。
> 5. ufusr_ask_unload函数用于设置立即卸载二次开发程序。
>
> 该代码通过NX Open UI接口实现了交互式指定平面的功能，用户可以输入平面参数或拾取点来指定平面。同时，代码中包含了错误处理和用户提示功能。
>
