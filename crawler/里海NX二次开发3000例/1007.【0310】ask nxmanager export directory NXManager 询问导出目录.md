### 【0310】ask nxmanager export directory NXManager 询问导出目录

#### 代码

```cpp
    /*HEAD ASK_UGMGR_EXPORT_DIRECTORY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_ugmgr.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，翻译如下：

注意：UF_print_syslog是V18版本中的新功能，只回答翻译，不要添加任何其他内容。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        logical  
            export_status;  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            export_dir_name[MAX_FSPEC_SIZE+1];  
        if (!UF_CALL(UF_UGMGR_ask_export_directory(part, export_dir_name)))  
        {  
            WRITE_S(export_dir_name);  
        }  
        if (!UF_CALL(UF_UGMGR_ask_file_export_status(&export_status)))  
        {  
            WRITE_L(export_status);  
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

> 这是NX Open的一个二次开发示例代码，主要功能是获取当前显示部件的导出目录和文件导出状态。
>
> 代码流程如下：
>
> 1. 包含了必要的NX Open头文件。
> 2. 定义了错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 3. 定义了写入逻辑值到列表窗口的函数write_logical_to_listing_window，用于显示布尔变量。
> 4. 定义了写入字符串到列表窗口的函数write_string_to_listing_window，用于显示字符串。
> 5. 定义了主要功能函数do_it：
>
> 1. 定义了ufusr函数，这是NX二次开发程序的入口函数。在这个函数中初始化NX Open环境，调用do_it函数执行功能，然后终止NX Open环境。
> 2. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示在程序退出时可以立即卸载NX Open库。
>
> 整体来说，这是一个简单的NX Open二次开发示例，演示了如何获取NX部件的导出设置，并打印到列表窗口。
>
