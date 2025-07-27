### 【0182】ask dialog directory 询问目录对话框

#### 代码

```cpp
    /*HEAD ASK_DIALOG_DIRECTORY CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译如下：

注意：UF_print_syslog 是 V18 中的新增功能，请只提供翻译，无需添加任何其他内容。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void ask_part_dialog_directory(char *fspec)  
    {  
        char  
            *p_dir = NULL;  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_PART_DIR, &p_dir));  
        if (!strcmp(p_dir, ""))  
        {  
            uc4565(1, fspec);  
            UF_CALL(UF_UI_set_dialog_directory(UF_UI_PART_DIR, fspec));  
        }  
        else  
            strcpy(fspec, p_dir);  
        UF_free(p_dir);  
    }  
    static void do_it(void)  
    {  
        char  
            dir_name[MAX_FSPEC_SIZE+1];  
        ask_part_dialog_directory(dir_name);  
        WRITE_S(dir_name);  
    }  
    /* 里海 */  
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

> 这段代码是NX二次开发代码，主要实现以下功能：
>
> 1. 错误报告函数：定义了一个report_error函数，用于在调用UF函数出错时输出错误信息到系统日志和提示窗口。
> 2. 输出信息函数：定义了write_string_to_listing_window函数，用于将字符串输出到NX的提示窗口。
> 3. 询问部件目录函数：定义了ask_part_dialog_directory函数，用于询问用户选择一个目录，如果没有选择，则使用默认目录，并更新NX的部件目录。
> 4. 主函数：定义了do_it函数，用于调用ask_part_dialog_directory函数，并将选择的目录输出到提示窗口。
> 5. NX启动函数：定义了ufusr函数，用于在NX启动时调用do_it函数。
> 6. 卸载函数：定义了ufusr_ask_unload函数，用于在卸载NX时返回立即卸载的标志。
>
> 总体来说，这段代码的主要功能是在NX启动时询问用户选择一个目录，如果没有选择，则使用默认目录，并更新NX的部件目录，同时将选择的目录输出到提示窗口。
>
