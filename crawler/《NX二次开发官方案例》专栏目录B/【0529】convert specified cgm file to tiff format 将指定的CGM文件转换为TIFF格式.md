### 【0529】convert specified cgm file to tiff format 将指定的CGM文件转换为TIFF格式

#### 代码

```cpp
    /*HEAD CONVERT_SPECIFIED_CGM_FILE_TO_TIFF_FORMAT CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是一个在V18版本中新增的函数。该函数用于打印系统日志，文档中提到：在V18版本中新增了UF_print_syslog函数，用于打印系统日志。 */  
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
            tif_spec[MAX_FSPEC_SIZE + 1];  
        while (prompt_for_cgm_file_name("Convert to tiff", cgm_spec))  
        {  
            strcpy(tif_spec, cgm_spec);  
            strcpy(strstr(tif_spec, ".cgm"), ".tif");  
            WRITE_S(cgm_spec);  
            WRITE_S(tif_spec);  
            WRITE("Converting...\n");  
            UF_CALL(UF_PLOT_convert_file(cgm_spec, UF_PLOT_TIFF_FORMAT, tif_spec));  
        }  
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

> 这是用于将CGM文件转换为TIFF格式的NX Open C++代码，主要功能如下：
>
> 1. 定义了一个错误报告函数report_error，用于输出函数调用失败时的错误信息。
> 2. 定义了一个组合目录和通配符的函数combine_directory_and_wildcard，用于生成文件选择对话框的过滤条件。
> 3. 定义了一个提示用户选择CGM文件名的函数prompt_for_cgm_file_name，它会调用NX的文件选择对话框让用户选择文件。
> 4. 定义了写入信息到列表窗口的宏WRITE和WRITE_S。
> 5. 定义了一个写入字符串到列表窗口的函数write_string_to_listing_window。
> 6. 定义了一个执行转换的函数do_it，它会提示用户选择CGM文件，然后将其转换为TIFF文件，并将转换过程中的信息输出到列表窗口。
> 7. 定义了ufusr函数，它是NX二次开发的入口函数，在这里调用do_it函数执行转换。
> 8. 定义了ufusr_ask_unload函数，用于在卸载时立即卸载。
>
> 总体来说，这段代码实现了在NX中打开一个对话框，让用户选择CGM文件，然后将该文件转换为TIFF格式的功能。转换过程的信息会输出到列表窗口。
>
