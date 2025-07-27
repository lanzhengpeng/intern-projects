### 【0428】ask new partname 请求新部件名称

#### 代码

```cpp
    /*HEAD ASK_NEW_PARTNAME CCC UFUN */  
    #include <stdio.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog 是 V18 版本中新增的功能。 */  
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
        int  
            units = UF_PART_ENGLISH,  
            resp;  
        char  
            filename[MAX_FSPEC_SIZE+1];  
        strcpy(filename, "");  
        UF_CALL(UF_UI_ask_create_part_filename(filename, &units, &resp));  
        printf("The specified filename is %s\n", filename);  
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

> 这段NX Open C++代码实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在发生错误时输出错误信息。
> 2. 定义了一个do_it函数，用于执行主要功能：使用UF_UI_ask_create_part_filename函数询问用户指定一个新部件的文件名，并将文件名保存到filename数组中。
> 3. 定义了ufusr函数，作为NX Open的入口函数。该函数首先调用UF_initialize初始化NX Open，然后调用do_it函数执行主要功能，最后调用UF_terminate终止NX Open。
> 4. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示该应用程序在关闭时会立即卸载。
>
> 整体而言，这段代码实现了在NX环境中通过对话框询问用户输入新部件文件名并打印输出的功能。代码结构清晰，使用了NX Open提供的UI和UF函数，是NX二次开发的一个典型示例。
>
