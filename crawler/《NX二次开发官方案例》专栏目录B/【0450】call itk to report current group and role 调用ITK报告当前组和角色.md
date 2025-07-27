### 【0450】call itk to report current group and role 调用ITK报告当前组和角色

#### 代码

```cpp
    /*HEAD CALL_ITK_TO_REPORT_CURRENT_GROUP_AND_ROLE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_ugmgr.h>  
    #define GET_CURRENT_GROUP 1  
    #define GET_CURRENT_ROLE 2  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档中的内容，UF_print_syslog是一个在V18版本中新增的函数。 */  
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
            output_code;  
        char  
            *input_string = { "" },  
            msg[MAX_LINE_SIZE+1],  
            *output_string = NULL;  
        UF_UI_open_listing_window();  
        UF_CALL(UF_UGMGR_invoke_pdm_server(GET_CURRENT_GROUP, input_string,  
            &output_code,    &output_string));  
        sprintf(msg,"Current Group from ITK = %s\n", output_string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
        if (output_string != NULL) free(output_string);  
        UF_CALL(UF_UGMGR_invoke_pdm_server(GET_CURRENT_ROLE, input_string,  
            &output_code, &output_string));  
        sprintf(msg,"Current Role from ITK = %s\n", output_string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
        if (output_string != NULL) free(output_string);  
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

> 这段NX二次开发代码的主要功能是获取当前用户在NX中的组和角色信息，并通过打印窗口显示出来。
>
> 代码的主要流程如下：
>
> 1. 定义了宏GET_CURRENT_GROUP和GET_CURRENT_ROLE，分别用于获取当前组和角色。
> 2. 定义了宏UF_CALL，用于调用NX的函数，并捕捉错误。
> 3. 定义了report_error函数，用于在发生错误时，打印错误信息。
> 4. 定义了do_it函数，用于获取当前组和角色信息，并打印显示。
> 5. ufusr函数是NX二次开发的入口函数，在这里调用do_it函数以执行主要功能。
> 6. ufusr_ask_unload函数用于在卸载用户二次开发程序时立即卸载。
>
> 该代码的主要功能是获取当前用户在NX中的组和角色信息，并通过打印窗口显示出来。通过调用NX提供的UF_UGMGR_invoke_pdm_server函数，可以轻松实现这一功能。
>
