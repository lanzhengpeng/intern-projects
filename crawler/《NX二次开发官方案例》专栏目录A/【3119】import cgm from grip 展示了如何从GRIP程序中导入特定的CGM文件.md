### 【3119】import cgm from grip 展示了如何从GRIP程序中导入特定的CGM文件

#### 代码

```cpp
    /*HEAD IMPORT_CGM_FROM_GRIP CCC UFUN */  
    /*  
        Here is sample GRIP code demonstating how to use this program to  
        import a specific CGM file from a GRIP program.  
            string/ cgm_filename(132)  
            data/ cgm_filename, 'parts/sample.cgm'  
            grargs/ cgm_filename  
            XSPAWN/ ufun, '/path/to/import_cgm_from_grip.sl'  
            halt  
    */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的功能。文档的目的是提醒注意UF_print_syslog是V18版本新增的功能，请仅回答译文，不要添加任何废话。 */  
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
    static void do_it(void)  
    {  
        char  
            cgm_filename[133],  
            error_text[MAX_LINE_SIZE+1];  
        UF_args_t  
            grip_arg;  
        grip_arg.type = UF_TYPE_CHAR;  
        grip_arg.length = 0;  
        grip_arg.address = cgm_filename;  
        UF_CALL(UF_ask_grip_args(1, &grip_arg));  
        UF_CALL(uc6411(cgm_filename, error_text));  
        if (strcmp(error_text, "")) WRITE_S(error_text);  
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

> 这段代码是一个NX的二次开发示例，其主要功能是从GRIP程序中导入指定的CGM文件。以下是代码的主要功能：
>
> 1. 包含头文件：代码首先包含了必要的头文件，如stdio.h、string.h、uf.h和uf_ui.h。
> 2. 错误处理函数：定义了report_error函数，用于处理UF函数调用返回的错误码，并在控制台和日志窗口中输出错误信息。
> 3. 字符串输出函数：定义了write_string_to_listing_window函数，用于在日志窗口输出字符串。
> 4. 主逻辑函数：定义了do_it函数，该函数从GRIP获取CGM文件路径，然后调用uc6411函数导入CGM文件，并输出导入过程中的错误信息。
> 5. UF初始化和终止：在ufusr函数中，首先调用UF_initialize初始化UF环境，然后调用do_it执行主逻辑，最后调用UF_terminate终止UF环境。
> 6. 卸载函数：定义了ufusr_ask_unload函数，返回立即卸载标志，表示该UF可以被立即卸载。
> 7. 注释：代码开头有注释，详细说明了如何从GRIP调用该UF，并给出了GRIP调用代码示例。
> 8. 函数指针：代码末尾有函数指针声明，表示ufusr是主函数入口。
>
> 综上所述，这段代码实现了从GRIP导入CGM文件的功能，具有错误处理、日志输出和卸载处理等功能，是一个典型的NX二次开发示例。
>
