### 【2963】test uistyler dialog 测试UIStyler对话框

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
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
    static logical prompt_for_uistyler_dialog_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            *uistyler_dir,  
            *uistyler_ext = "*.dlg",  
            filter[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_UFUN_DIR, &uistyler_dir));  
        combine_directory_and_wildcard(uistyler_dir, uistyler_ext, filter);  
        UF_free(uistyler_dir);  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "UIStyler Dialog", filter, "",  
            fspec, &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            resp;  
        char  
            fspec[MAX_FSPEC_SIZE+1];  
        while (prompt_for_uistyler_dialog_name("Test UIStyler Dialog", fspec))  
            UF_CALL(UF_STYLER_create_dialog(fspec, NULL, 0, NULL, &resp));  
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

> 这段代码是一个NX OpenUF函数库的应用示例，主要用于创建UIStyler对话框。下面是该代码的主要功能介绍：
>
> 1. 包含了必要的头文件，如stdio.h、string.h、uf.h等。
> 2. 定义了ECHO函数，用于打印信息到日志窗口。
> 3. 定义了UF_CALL宏，用于调用UF函数并报告错误。
> 4. 定义了report_error函数，用于打印UF函数调用的错误信息。
> 5. 定义了combine_directory_and_wildcard函数，用于组合目录和通配符。
> 6. 定义了prompt_for_uistyler_dialog_name函数，用于提示用户输入UIStyler对话框文件名，并返回文件路径。
> 7. 定义了do_it函数，用于不断提示用户输入文件名，然后创建UIStyler对话框。
> 8. 定义了ufusr函数，这是NX二次开发的标准入口点。初始化UF，调用do_it，然后终止UF。
> 9. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 总的来说，这段代码实现了通过用户输入的文件名，不断创建UIStyler对话框的功能。
>
