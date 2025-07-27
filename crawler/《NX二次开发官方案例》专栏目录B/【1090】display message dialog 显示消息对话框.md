### 【1090】display message dialog 显示消息对话框

#### 代码

```cpp
    /*HEAD DISPLAY_MESSAGE_DIALOG CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：注意：UF_print_syslog 是 V18 的新功能，只回答翻译，不要说废话。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        int  
            response = 0;  
        char  
            *messages[3] = { "Message One", "Message Two", "Message Three" };  
        UF_UI_message_buttons_t   
            buttons;  
        buttons.button1 = TRUE; /* OK button 里海译:好的按钮 */  
        buttons.button2 = TRUE; /* BACK button 里海译:返回按钮 */  
        buttons.button3 = TRUE; /* CANCEL button 里海译:取消按钮 */  
        buttons.label1 = "MY_OK";  
        buttons.label2 = "MY_BACK";  
        buttons.label3 = NULL;   /* will use the default value 里海译:将会使用默认值。 */  
        buttons.response1 = 2;  
        buttons.response2 = 4;  
        buttons.response3 = 6;  
        UF_CALL( UF_UI_message_dialog( "My Dialog Title",   
                                       UF_UI_MESSAGE_INFORMATION,   
                                       messages, 3, FALSE,   
                                       &buttons, &response ));  
        WRITE_D( response );  
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

> 这段代码是一个NX Open的UF程序，用于实现一个消息对话框。以下是该代码的主要功能和实现步骤：
>
> 1. 定义错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 2. 定义整数写入函数write_integer_to_listing_window，用于将整数输出到列表窗口。
> 3. 实现消息对话框函数do_it，包括以下步骤：定义3个消息字符串。设置按钮属性，包括显示“确定”、“返回”和“取消”按钮。设置按钮标签和响应值。调用UF_UI_message_dialog函数显示对话框，并获取用户点击的按钮响应值。
> 4. 定义3个消息字符串。
> 5. 设置按钮属性，包括显示“确定”、“返回”和“取消”按钮。
> 6. 设置按钮标签和响应值。
> 7. 调用UF_UI_message_dialog函数显示对话框，并获取用户点击的按钮响应值。
> 8. 定义ufusr函数，在NX启动时调用do_it函数，并最终结束NX。
> 9. 定义ufusr_ask_unload函数，返回立即卸载标志。
>
> 该代码的主要功能是创建一个包含自定义按钮和响应值的NX消息对话框。当用户点击按钮时，会获取对应的响应值，并输出到列表窗口。
>
