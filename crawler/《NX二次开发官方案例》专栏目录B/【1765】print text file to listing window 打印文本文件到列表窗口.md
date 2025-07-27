### 【1765】print text file to listing window 打印文本文件到列表窗口

#### 代码

```cpp
    /*HEAD  PRINT_TEXT_FILE_TO_LISTING_WINDOW CCC UFUN */  
    /* This is the GRIP which prints a text file to the listing window  
        NUMBER/ orig_ldev  
        STRING / file_name (132)  
        UFARGS / file_name  
            orig_ldev = &LDEV  
            &LDEV = &CRT  
            FETCH/ TXT, 10, file_name  
            FPRINT/ 10  
            FTERM/ TXT, 10  
            &LDEV = orig_ldev  
        HALT  
     */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译如下：

注意：UF_print_syslog是V18版本新增的。 */  
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
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
        char file_name[MAX_FSPEC_SIZE+1]="";  
        UF_args_t    grip_arg_list[1];  
        grip_arg_list[0].type    = UF_TYPE_CHAR;  
        grip_arg_list[0].length  = 0;  
        grip_arg_list[0].address = file_name;  
        while (prompt_for_text("Enter File name", file_name))  
        {  
    #ifdef _WIN32  
            UF_CALL(UF_call_grip("n:\\grip\\fprint.grx", 1, grip_arg_list));  
    #else  
            UF_CALL(UF_call_grip("grip/fprint.grx", 1, grip_arg_list));  
    #endif  
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

> 这段NX二次开发代码实现了以下功能：
>
> 1. 定义了一个宏UF_CALL，用于调用UF函数并报告错误。
> 2. 定义了一个错误报告函数report_error，用于输出错误信息。
> 3. 定义了一个获取用户输入的函数prompt_for_text。
> 4. 定义了主函数do_it，用于循环获取用户输入的文件名，并调用GRIP来打印文件内容到列表窗口。
> 5. 定义了ufusr函数，作为NX的二次开发入口函数，用于初始化和调用do_it。
> 6. 定义了ufusr_ask_unload函数，用于卸载二次开发程序。
> 7. 使用UF_CALL宏调用UF函数，并通过report_error函数报告错误。
> 8. 使用prompt_for_text函数获取用户输入的文件名。
> 9. 使用UF_call_grip函数调用GRIP来打印文件内容。
> 10. 通过UF_UI函数操作NX的列表窗口。
>
> 总体来说，这段代码实现了让用户输入文件名，并调用GRIP打印文件内容到NX的列表窗口的功能。通过使用UF_CALL宏和错误报告函数，提高了代码的健壮性。
>
