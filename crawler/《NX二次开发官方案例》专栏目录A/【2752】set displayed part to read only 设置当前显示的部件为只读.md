### 【2752】set displayed part to read only 设置当前显示的部件为只读

#### 代码

```cpp
    /*HEAD SET_DISPLAYED_PART_TO_READ_ONLY CCC UFUN */  
    #define _INCLUDE_POSIX_SOURCE  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <sys/stat.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 新增的功能。 */  
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
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            path[MAX_FSPEC_SIZE+1];  
        UF_PART_ask_part_name(part, path);  
        chmod(path, (S_IRUSR | S_IRGRP | S_IROTH));  
        UF_PART_save(); /* To update graphics to show (Read Only) status 里海译:更新图形以显示只读状态（Read Only）。 */  
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

> 这段代码是一个NX Open的二次开发示例，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于输出函数调用失败时的错误信息。
> 2. 定义了一个do_it函数，该函数获取当前NX显示的零件，查询其文件路径，然后使用chmod系统调用将其设置为只读权限。最后调用UF_PART_save函数保存NX文件，更新图形以显示只读状态。
> 3. 定义了ufusr函数，这是NX二次开发的入口函数。该函数首先初始化NX Open，然后调用do_it函数实现功能，最后终止NX Open。
> 4. 定义了ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY表示在NX关闭时立即卸载该二次开发应用程序。
>
> 综上所述，这段代码通过NX Open API实现了将当前NX显示的零件设置为只读状态的功能，并通过UF_print_syslog等函数输出了详细的错误信息。
>
