### 【2443】report work part name The work part name is currently unavailable.

#### 代码

```cpp
    /*HEAD REPORT_WORK_PART_NAME CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是 V18 中的新功能。翻译为：UF_print_syslog 是 V18 中的新增功能。 */  
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
            part = UF_ASSEM_ask_work_part();  
        char  
            messg[MAX_FSPEC_SIZE+18],  
            work_fspec[MAX_FSPEC_SIZE+1];  
        if (part != NULL_TAG)  
        {  
            UF_PART_ask_part_name(part, work_fspec);  
            sprintf(messg, "The work part is %s", work_fspec);  
            uc1601(messg, TRUE);  
        }  
        else uc1601("The is currently no work part", TRUE);  
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

> 这段NX二次开发代码的功能是获取当前工作部件名称并显示。以下是代码的详细解释：
>
> 1. 头文件包含：引入了必要的NX Open API头文件，如uf.h、uf_ui.h、uf_assem.h和uf_part.h。
> 2. 错误报告函数：定义了一个错误报告函数report_error，用于在出错时打印错误信息，包括文件名、行号、函数名、错误码和错误消息。
> 3. 获取工作部件名称函数：定义了一个获取工作部件名称的函数do_it。首先通过UF_ASSEM_ask_work_part()获取当前工作部件的tag，然后通过UF_PART_ask_part_name()获取部件名称，并将其显示。
> 4. 主函数ufusr：首先调用UF_initialize()初始化NX Open API，然后调用do_it()获取并显示工作部件名称，最后调用UF_terminate()终止NX Open API。
> 5. 卸载请求函数：定义了一个卸载请求函数ufusr_ask_unload，返回UF_UNLOAD_IMMEDIATELY表示立即卸载。
> 6. 使用宏定义UF_CALL：通过宏定义UF_CALL封装函数调用，以便在出错时自动报告错误。
> 7. 使用UF_print_syslog打印日志：在出错时调用UF_print_syslog打印错误日志到NX系统日志。
> 8. 使用UF_UI_write_listing_window写日志窗口：如果无法打开日志窗口，则将错误信息写入日志窗口。
>
> 总的来说，这段代码通过NX Open API获取当前工作部件名称并显示，同时在出错时提供了详细的错误报告和日志打印功能。
>
