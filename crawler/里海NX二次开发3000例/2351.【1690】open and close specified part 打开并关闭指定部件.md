### 【1690】open and close specified part 打开并关闭指定部件

#### 代码

```cpp
    /*HEAD OPEN_AND_CLOSE_SPECIFIED_PART CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，以下是关键信息的翻译：

UF_print_syslog是V18版本新增的函数。 */  
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
    static void report_load_status(UF_PART_load_status_t *status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        for (ii=0; ii<status->n_parts; ii++)  
        {  
            UF_get_fail_message(status->statuses[ii], msg);  
            printf("    %s - %s\n", status->file_names[ii], msg);  
        }  
        if (status->n_parts > 0)  
        {  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            resp;  
        logical  
            disp = FALSE;  
        tag_t  
            part,  
            orig_part = UF_PART_ask_display_part();  
        char  
            file_name[MAX_FSPEC_SIZE + 1] = { "" };  
        UF_PART_load_status_t  
            status;  
        if (!(UF_CALL(UF_UI_ask_open_part_filename(file_name, &disp, &resp))) &&  
             (resp == UF_UI_OK))  
        {  
            UF_CALL(UF_PART_open(file_name, &part, &status));  
            if (status.n_parts > 0) report_load_status(&status);  
            UF_CALL(UF_PART_close(part, 1, 1));  
            if (orig_part != NULL_TAG)  
                UF_CALL(UF_PART_set_display_part(orig_part));  
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

> 这段NX二次开发代码主要实现了以下功能：
>
> 1. 包含了NX Open API的头文件，用于调用NX的函数。
> 2. 定义了一个错误报告函数report_error，用于在调用NX函数失败时打印错误信息。
> 3. 定义了一个状态报告函数report_load_status，用于报告零件加载状态。
> 4. 定义了一个执行函数do_it，用于执行打开和关闭指定零件的主要逻辑：
>
> 1. 定义了ufusr函数作为NX二次开发的入口函数，在初始化NX Open API后调用do_it函数，并在结束后释放资源。
> 2. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示在二次开发模块卸载时无需等待。
>
> 综上所述，这段代码通过交互方式让用户指定要打开的零件文件，并打开、关闭指定的零件，同时处理了打开失败的情况，实现了零件的打开和关闭功能。
>
