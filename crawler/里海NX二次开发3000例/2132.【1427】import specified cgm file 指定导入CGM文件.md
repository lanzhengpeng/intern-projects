### 【1427】import specified cgm file 指定导入CGM文件

#### 代码

```cpp
    /*HEAD IMPORT_SPECIFIED_CGM_FILE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_std.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，翻译为：

注意：UF_print_syslog在V18中是新增的。 */  
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
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    static void do_it(void)  
    {  
        char  
            cfn[UF_CFI_MAX_PATH_NAME_SIZE],  
            error_text[133];  
        while (prompt_for_cgm_file_name("Import", cfn))  
            if (UF_CALL(uc6411(cfn, error_text)))  
            {  
                WRITE_S(error_text);  
            }  
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

> 这段代码是NX的二次开发代码，主要功能是导入CGM文件。以下是代码的详细介绍：
>
> 1. 头文件包含：代码包含了必要的NX二次开发头文件，如uf.h、uf_ui.h等。
> 2. 错误报告宏：定义了UF_CALL宏，用于在调用NX API函数后检查返回码，并报告错误。
> 3. 错误报告函数：report_error函数用于打印错误信息，包括文件名、行号、函数名、返回码和错误描述。
> 4. 提示框宏：定义了WRITE_S宏，用于在列表窗口中输出字符串。
> 5. 字符串写入函数：write_string_to_listing_window用于将字符串输出到列表窗口。
> 6. 目录和通配符组合函数：combine_directory_and_wildcard用于组合目录和通配符，生成文件选择框的过滤条件。
> 7. 文件名提示函数：prompt_for_cgm_file_name用于弹出文件选择框，提示用户选择CGM文件。
> 8. 主函数：do_it函数用于循环提示用户选择CGM文件，并调用NX API函数uc6411导入文件，导入成功则输出提示信息。
> 9. ufusr主函数：ufusr是NX二次开发的入口函数，用于初始化和终止NX环境，并调用do_it函数。
> 10. 卸载函数：ufusr_ask_unload用于设置二次开发模块的卸载方式。
>
> 综上所述，这段代码实现了NX环境下CGM文件的导入功能，包括文件选择、错误处理、信息提示等。
>
