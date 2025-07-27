### 【1422】import expressions and update model 导入表达式并更新模型

#### 代码

```cpp
    /*HEAD IMPORT_EXPRESSIONS_AND_UPDATE_MODEL CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_modl.h>  
    #include <uf_modl_expressions.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能。 */  
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
    static void combine_directory_and_wildcard(char *dir, char *fltr, char *spec)  
    {  
        if (!strcmp(dir, ""))  
            strcpy(spec, fltr);  
        else  
        {  
            UF_CALL(uc4575(dir, 0, "junk.xxx", spec));  
            strcpy(strstr(spec, "junk.xxx"), fltr);  
        }  
    }  
    static logical prompt_for_exp_file_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            *exp_dir,  
            *exp_ext,  
            filter[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_IMPORT_DIR, &exp_dir));  
        UF_CALL(UF_UI_ask_dialog_filter(UF_UI_IMPORT_EXPRESSIONS_FLTR, &exp_ext));  
        combine_directory_and_wildcard(exp_dir, exp_ext, filter);  
        UF_free(exp_dir);  
        UF_free(exp_ext);  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "Expressions File Name", filter, "",  
            fspec, &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        char  
            efn[UF_CFI_MAX_PATH_NAME_SIZE] = { "" };  
        while (prompt_for_exp_file_name("Specify expressions file to import", efn))  
            if (!UF_CALL(UF_MODL_import_exp(efn, 0))) UF_CALL(UF_MODL_update());  
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

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 导入表达式文件，更新模型
> 2. 通过对话框提示用户指定表达式文件，并导入该文件
> 3. 使用UF_MODL_import_exp函数导入表达式文件，使用UF_MODL_update更新模型
> 4. 使用UF_print_syslog打印错误信息到系统日志
> 5. 使用UF_UI函数创建文件对话框，提示用户选择文件
> 6. 定义了错误处理函数report_error，在调用NX API时捕获错误并打印信息
> 7. 在ufusr函数中初始化和终止NX环境，调用do_it函数导入表达式文件
> 8. 定义了ufusr_ask_unload函数，在插件卸载时立即卸载
>
> 总的来说，这段代码通过对话框提示用户选择表达式文件，调用NX API导入并更新模型，同时提供了错误处理机制。
>
