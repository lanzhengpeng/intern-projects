### 【1423】import external spreadsheet 导入外部电子表格

#### 代码

```cpp
    /*HEAD IMPORT_EXTERNAL_SPREADSHEET CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_xs.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18中新增的函数，用于打印系统日志消息。 */  
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
    static logical pick_one_of_two_choices(char *prompt, char *option_one,  
        char *option_two, int *choice)  
    {  
        int  
            resp;  
        char  
            options[2][38];  
        strncpy(&options[0][0], option_one, 37);  
        strncpy(&options[1][0], option_two, 37);  
        options[0][37] = '\0';  
        options[1][37] = '\0';  
        resp = uc1603(prompt, *choice, options, 2);  
        if ((resp > 4) && (resp < 19))  
        {  
            *choice = resp - 4;  /* returns 1 or 2 里海译:returns 1 or 2 翻译为返回1或2。 */  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            resp,  
            which = 1;  
        char  
            filter[UF_CFI_MAX_PATH_NAME_SIZE] = { "*.xls" },  
            *opts[2] = { "MODELING_SHEET", "DEFAULT_SHEET" },  
            sfn[UF_CFI_MAX_PATH_NAME_SIZE] = { "" };  
        if (!UF_CALL(UF_UI_create_filebox("Import spreadsheet data",  
                "Spreadsheet file", filter, "", sfn, &resp)) &&  
            (resp != UF_UI_CANCEL) &&  
            pick_one_of_two_choices("Which spreadsheet?", "Modeling", "Gateway",  
                &which) &&  
            !UF_CALL(UF_XS_store_spreadsheet(opts[which-1], sfn))  
            && (which == 1))  
                UF_CALL(UF_MODL_update());  
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

> 这段代码是用于NX的二次开发，主要用于导入外部Excel表格数据。主要功能如下：
>
> 1. 包含了必要的NX头文件，如uf.h, uf_ui.h等。
> 2. 定义了宏UF_CALL，用于检测NX函数调用是否成功，并在失败时报告错误。
> 3. 实现了report_error函数，用于在函数调用失败时打印错误信息。
> 4. 实现了pick_one_of_two_choices函数，用于弹出一个对话框，让用户在两个选项之间进行选择。
> 5. 实现了do_it函数，用于导入Excel表格数据。该函数首先通过文件对话框让用户选择Excel文件，然后弹出一个对话框让用户选择导入的表格（建模表格或默认表格），最后调用UF_XS_store_spreadsheet函数将选择的表格数据导入NX。
> 6. 实现了ufusr函数，这是NX二次开发的入口函数。在该函数中，首先调用UF_initialize函数初始化NX，然后调用do_it函数执行导入操作，最后调用UF_terminate函数结束NX。
> 7. 实现了ufusr_ask_unload函数，用于设置卸载二次开发模块的方式。
>
> 总的来说，这段代码通过交互式的方式，让用户选择Excel文件和表格，然后将其导入NX。通过使用NX提供的API函数，实现了与NX的交互。
>
