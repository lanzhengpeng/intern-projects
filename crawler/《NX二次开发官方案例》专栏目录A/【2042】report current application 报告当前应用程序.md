### 【2042】report current application 报告当前应用程序

#### 代码

```cpp
    /*HEAD REPORT_CURRENT_APPLICATION CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
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
    static void report_current_application(void)  
    {  
        int  
            ii,  
            module,  
            a_module;  
        char  
            a_line[MAX_LINE_SIZE+1],  
            api_dir[UF_CFI_MAX_PATH_NAME_SIZE],  
            *base_dir,  
            define[133],  
            module_name[133],  
            *ptr,  
            ufh_file[UF_CFI_MAX_PATH_NAME_SIZE];  
        FILE  
            *in_file;  
        UF_CALL(UF_ask_application_module(&module));  
        UF_CALL(UF_translate_variable("UGII_BASE_DIR", &base_dir));  
        UF_CALL(uc4575(base_dir, 100, "ugopen", api_dir));  
        UF_CALL(uc4575(api_dir, 0, "uf.h", ufh_file));  
        if ((in_file = fopen(ufh_file, "r")) == NULL)  
        {  
            WRITE_D(module);  
            ECHO("(uf.h not found - cannot get module name)\n");  
            return;  
        }  
        while (!feof(in_file))  
        {  
            fgets(a_line, MAX_LINE_SIZE, in_file);  
            if ((ii = sscanf(a_line, "#define UF_APP_%s (%d) /* %[^*] ", define,  
                &a_module, module_name)) == 3)  
            {  
                if (a_module == module)  
                {  
                    ECHO(module_name);  
                    ECHO("\n");  
                    break;  
                }  
            }  
        }  
        fclose(in_file);  
    }  
    static void do_it(void)  
    {  
        report_current_application();  
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

> 这段代码是用于NX的二次开发，主要功能是获取并打印当前打开的NX应用程序模块名称。代码的主要组成部分包括：
>
> 1. 头文件包含：包含了必要的头文件，如stdio.h、stdlib.h、string.h等，以及NX的UF和UF_UI头文件。
> 2. 错误报告宏：定义了两个宏ECHO和UF_CALL，用于打印错误信息。
> 3. 错误报告函数：定义了report_error函数，用于在UF函数调用出错时打印错误信息。
> 4. 获取当前模块函数：定义了report_current_application函数，用于获取并打印当前打开的NX应用程序模块名称。
> 5. 主函数：定义了do_it函数，调用report_current_application函数获取并打印当前模块。
> 6. ufusr函数：这是NX二次开发的标准入口函数，初始化UF环境，调用do_it函数，然后终止UF环境。
> 7. ufusr_ask_unload函数：NX要求实现的卸载函数，返回立即卸载标志。
>
> 总体来说，这段代码的主要功能是获取并打印当前打开的NX应用程序模块名称。它通过调用NX的UF函数来实现，并且在出错时能够打印错误信息。
>
