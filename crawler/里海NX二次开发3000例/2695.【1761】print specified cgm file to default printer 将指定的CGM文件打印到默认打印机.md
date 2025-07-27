### 【1761】print specified cgm file to default printer 将指定的CGM文件打印到默认打印机

#### 代码

```cpp
    /*HEAD PRINT_SPECIFIED_CGM_FILE_TO_DEFAULT_PRINTER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_plot.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，可以得知UF_print_syslog是在V18版本中新增的功能。 */  
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
    static void combine_directory_and_wildcard(char *dir, char *fltr, char *spec)  
    {  
        if (!strcmp(dir, ""))  
            strcpy(spec, fltr);  
        else  
        {  
            UF_CALL(uc4575(dir, 0, "junk.xxx", spec));  
            strcpy(strstr(spec, "junk.xxx"), fltr);  
        }  
    }  
    static logical prompt_for_cgm_file_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            *cgm_dir,  
            *cgm_ext,  
            filter[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_IMPORT_DIR, &cgm_dir));  
        UF_CALL(UF_UI_ask_dialog_filter(UF_UI_IMPORT_CGM_FLTR, &cgm_ext));  
        combine_directory_and_wildcard(cgm_dir, cgm_ext, filter);  
        UF_free(cgm_dir);  
        UF_free(cgm_ext);  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "CGM File Name", filter, "",  
            fspec, &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
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
            cgm_spec[MAX_FSPEC_SIZE + 1],  
            *printer,  
            *profile;  
        UF_CALL(UF_PLOT_ask_default_printer_and_profile( &printer, &profile));  
        WRITE("Send print to:  ");  
        WRITE_S(printer);  
        WRITE("Using:  ");  
        WRITE_S(profile);  
        while (prompt_for_cgm_file_name("Print file", cgm_spec))  
        {  
            WRITE_S(cgm_spec);  
            UF_CALL(UF_PLOT_print_file(cgm_spec, printer, profile, 1));  
        }  
        UF_free(printer);  
        UF_free(profile);  
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

> 这段代码是NX的二次开发代码，主要功能是打印指定的CGM文件到默认打印机。
>
> 代码的主要逻辑包括：
>
> 1. 包含必要的NX API头文件，如uf.h, uf_ui.h等。
> 2. 定义了UF_CALL宏，用于调用NX API并检查错误，并在出错时报告错误信息。
> 3. combine_directory_and_wildcard函数用于组合目录名和通配符，生成文件筛选器。
> 4. prompt_for_cgm_file_name函数用于弹出一个对话框，提示用户选择一个CGM文件。
> 5. write_string_to_listing_window函数用于将字符串输出到NX的日志窗口。
> 6. do_it函数是主函数，首先获取默认打印机和打印配置文件，然后循环调用prompt_for_cgm_file_name获取要打印的CGM文件，并调用UF_PLOT_print_file打印文件。
> 7. ufusr函数是NX二次开发的标准入口函数，其中初始化NX环境，调用do_it函数，然后终止NX环境。
> 8. ufusr_ask_unload函数用于处理NX退出时的卸载操作。
>
> 整体来看，该代码利用NX的打印API实现了打印CGM文件到默认打印机的功能，代码结构清晰，各函数功能明确。
>
