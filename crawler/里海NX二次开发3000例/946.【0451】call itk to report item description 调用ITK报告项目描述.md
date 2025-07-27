### 【0451】call itk to report item description 调用ITK报告项目描述

#### 代码

```cpp
    /*HEAD CALL_ITK_TO_REPORT_ITEM_DESCRIPTION CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是一个在V18版本中新增的函数。 */  
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
    int main(int argc, char *argv[])  
    {  
        int  
            output_code;  
        char  
            part_name[132],  
            *output_string;  
        UF_CALL(UF_UGMGR_initialize(argc,(const char**)argv));  
        printf("Enter the WSOM_search name : ");  
        scanf("%s", part_name);  
        UF_CALL(UF_UGMGR_invoke_pdm_server(2, part_name, &output_code,  
            &output_string));  
        if (output_string != NULL) free(output_string);  
        return 0;  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是NX二次开发代码，主要实现了以下功能：
>
> 1. 包含了必要的头文件，如stdio.h、stdlib.h、string.h等，以及NX提供的uf.h、uf_ui.h、uf_ugmgr.h等头文件。
> 2. 定义了一个宏UF_CALL，用于执行NX函数，并在函数调用失败时输出错误信息。
> 3. 实现了一个report_error函数，用于打印函数调用失败时的错误代码和错误信息。
> 4. 在main函数中，初始化NX环境，提示用户输入要搜索的零件名，然后调用NX的UF_UGMGR_invoke_pdm_server函数在PDM系统中搜索该零件，并输出搜索结果。
> 5. 在搜索结果非空时，释放了动态分配的内存。
> 6. 实现了一个ufusr_ask_unload函数，返回立即卸载标志。
>
> 总体来说，这段代码实现了在NX环境下通过PDM系统搜索零件的功能，并在函数调用失败时提供了错误处理。代码结构清晰，注释详细，便于理解和使用。
>
