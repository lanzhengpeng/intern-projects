### 【0193】ask executable dir 询问可执行文件目录

#### 代码

```cpp
    /*Ask_Executable_Dir */  
    //File > Execute > NX Open..  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本中的新功能，用于打印系统日志。

翻译后的内容如下：
UF_print_syslog是V18版本中的新功能，用于打印系统日志。 */  
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
    static void Ask_Executable_Dir(char *fspec)  
    {  
        char  
            *p_dir = NULL;  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_UFUN_DIR, &p_dir));  
        if (!strcmp(p_dir, ""))  
        {  
            uc4565(1, fspec);  
            UF_CALL(UF_UI_set_dialog_directory(UF_UI_UFUN_DIR, fspec));  
        }  
        else  
            strcpy(fspec, p_dir);  
        UF_free(p_dir);  
    }  
    static void do_it(void)  
    {  
        char  
            dir_name[MAX_FSPEC_SIZE+1];  
        Ask_Executable_Dir(dir_name);  
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

> 这段代码是一个NX Open应用程序，其主要功能是获取NX可执行文件的目录路径并打印到列表窗口中。
>
> 主要步骤包括：
>
> 1. 包含必要的头文件，如stdio.h、stdlib.h、string.h、uf.h、uf_ui.h等。
> 2. 定义了一个错误报告函数report_error，用于输出错误信息。
> 3. 定义了一个写字符串到列表窗口的函数write_string_to_listing_window。
> 4. 定义了一个获取可执行文件目录的函数Ask_Executable_Dir，通过UF_UI_ask_dialog_directory获取目录路径，如果为空则使用默认路径，并更新对话框的目录。
> 5. 定义了一个执行函数do_it，调用Ask_Executable_Dir获取路径并打印到列表窗口。
> 6. 定义了ufusr函数，这是NX Open应用程序的入口点，在这里初始化UF模块，调用do_it函数，然后终止UF模块。
> 7. 定义了ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY表示程序可以立即卸载。
>
> 总的来说，这段代码通过二次开发获取NX可执行文件的目录路径，并打印出来，展示了NX Open应用程序的基本结构。
>
