### 【2957】test for file existence 文件存在检测

#### 代码

```cpp
    /*HEAD TEST_FOR_FILE_EXISTENCE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    static logical file_exists(char *file_spec)  
    {  
        int  
            in_session,  
            missing;  
        UF_CALL(UF_CFI_ask_file_exist(file_spec, &missing));  
        in_session = UF_PART_is_loaded(file_spec);  
        if (missing && (in_session != 1) && (in_session != 2))  
            return FALSE;  
        else  
            return TRUE;  
    }  
    static void do_it(void)  
    {  
        char  
            fn[UF_CFI_MAX_PATH_NAME_SIZE] = { "" },  
            *option;  
        UF_CALL(UF_translate_variable("UGII_OPTION", &option));  
        WRITE("UGII_OPTION=");  
        WRITE(option);  
        WRITE("\n");  
        while (prompt_for_text("Enter file name", fn))  
        {  
            WRITE_L(file_exists(fn));  
            WRITE_L(!uc4560(fn, 0));  
        /*  UF_CFI_ask_file_exist and uc4560 do not work correctly for  
            drive specifications such as "C:\" - see PR 1709449  
        */  
            if ((uc4508(fn, (1 << 13), 0, "")) >= 0)  
            {  
                ECHO("Directory exists\n");  
                uc4548();  
            }  
        }  
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

> 这段代码是一个NX的二次开发示例，其主要功能包括：
>
> 1. 定义了错误报告宏report_error，用于在调用NX API失败时报告错误信息。
> 2. 定义了逻辑值输出宏WRITE_L，用于将逻辑值输出到列表窗口。
> 3. 定义了文件是否存在函数file_exists，通过调用NX的文件接口来判断文件是否存在。
> 4. 定义了提示输入文件名函数prompt_for_text，用于提示用户输入文件名。
> 5. 主函数do_it实现了以下功能：输出UGII_OPTION环境变量的值。循环提示用户输入文件名，调用file_exists函数判断文件是否存在，并输出结果。判断输入的文件名是否为目录，如果是，输出提示信息。
> 6. 输出UGII_OPTION环境变量的值。
> 7. 循环提示用户输入文件名，调用file_exists函数判断文件是否存在，并输出结果。
> 8. 判断输入的文件名是否为目录，如果是，输出提示信息。
> 9. ufusr是NX二次开发程序的入口函数，初始化NX环境后调用do_it函数，最后终止NX环境。
> 10. ufusr_ask_unload函数用于设置在二次开发程序卸载时的行为。
>
> 总体来说，这段代码展示了NX二次开发中常用的错误处理、环境变量获取、文件操作、用户交互等基本功能，具有一定的参考价值。
>
