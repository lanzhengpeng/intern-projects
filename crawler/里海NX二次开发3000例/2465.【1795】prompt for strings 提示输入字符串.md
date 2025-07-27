### 【1795】prompt for strings 提示输入字符串

#### 代码

```cpp
    /*HEAD PROMPT_FOR_STRINGS CCC UFUN */  
    #include <stdio.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：UF_print_syslog是在V18版本中新增的功能。 */  
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
            ii,  
            ijunk[4],  
            resp,  
            strings[4] = { 301, 301, 301, 301 };  
        double  
            djunk[4];  
        char  
            menu[4][16] = { "First", "Second", "Third", "Fourth" },  
            user_input[4][31] = { "", "", "", "" };  
        while ((resp = uc1613("Enter strings", menu, 4, ijunk, djunk, user_input,  
            strings)) > 2)  
        {  
            for (ii = 0; ii < 4; ii++)  
            {  
                printf("%s = %s\n", &menu[ii][0], &user_input[ii][0]);  
                strcpy(&user_input[ii][0], "");  
            }  
        }  
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

> 这段代码是一个NX二次开发程序，主要功能是提示用户输入4个字符串，并显示这些字符串。以下是代码的主要逻辑：
>
> 1. 包含必要的头文件，如stdio.h、string.h、uf.h、uf_ui.h。
> 2. 定义了一个报告错误的函数report_error，用于输出函数调用失败时的错误信息。
> 3. 定义了do_it函数，用于提示用户输入字符串，并显示这些字符串。
> 4. 在do_it函数中，使用了一个while循环，循环调用uc1613函数，获取用户输入的字符串，直到用户输入了小于等于2的响应值。
> 5. 循环内，遍历用户输入的4个字符串，打印到控制台，并清空用户输入的字符串。
> 6. 定义了ufusr函数，这是NX二次开发的入口函数。在这个函数中，初始化NX环境，调用do_it函数，然后终止NX环境。
> 7. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示程序可以立即被卸载。
>
> 总的来说，这段代码通过提示用户输入字符串，并打印到控制台，实现了简单的字符串输入输出功能。
>
