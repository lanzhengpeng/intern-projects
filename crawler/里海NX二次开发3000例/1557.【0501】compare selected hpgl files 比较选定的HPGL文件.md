### 【0501】compare selected hpgl files 比较选定的HPGL文件

#### 代码

```cpp
    /*HEAD COMPARE_SELECTED_HPGL_FILES CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：

注意：UF_print_syslog是在V18版本中新增的，只回答译文，不要发表无关的言论。 */  
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
    static logical prompt_for_hpgl_file_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            *hpgl_dir,  
            *hpgl_ext = "*.hpgl",  
            filter[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_IMPORT_DIR, &hpgl_dir));  
        combine_directory_and_wildcard(hpgl_dir, hpgl_ext, filter);  
        UF_free(hpgl_dir);  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "HPGL File Name", filter, "",  
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
    static void do_it(void)  
    {  
        char  
            one_spec[MAX_FSPEC_SIZE + 1],  
            two_spec[MAX_FSPEC_SIZE + 1],  
            res_spec[MAX_FSPEC_SIZE + 1];  
        UF_PLOT_ughpgl_cmp_options_t  
            opts = { UF_PLOT_CMP_COLOR_WIDTH, UF_PLOT_CREATE_CMP_PLOT, 0.0 };  
        UF_PLOT_diff_cmp_status_t  
            different;  
        while (prompt_for_hpgl_file_name("First HPGL to Compare", one_spec) &&  
               prompt_for_hpgl_file_name("Second HPGL to Compare", two_spec) &&  
               prompt_for_hpgl_file_name("Comparison Results", res_spec))  
        {  
            WRITE("Comparing\n  ");  
            WRITE(one_spec);  
            WRITE("\nwith\n  ");  
            WRITE(two_spec);  
            WRITE("\n\n");  
            UF_CALL(UF_PLOT_compare_ughpgl_files(one_spec, two_spec, &opts,  
                res_spec, &different));  
            WRITE_L(different);  
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

> 这段NX二次开发代码的主要功能是提供用户界面，让用户可以比较两个HPGL文件，并输出比较结果。
>
> 主要功能包括：
>
> 1. 提供用户界面，让用户可以输入两个HPGL文件路径和一个输出结果文件路径。
> 2. 使用UF_PLOT_compare_ughpgl_files函数比较两个HPGL文件。
> 3. 输出比较结果到屏幕和日志文件。
> 4. 提供错误处理，将UF函数调用错误输出到日志文件。
> 5. 使用宏WRITE、WRITE_S、WRITE_L简化日志输出。
> 6. 主函数ufusr初始化NX环境，调用do_it函数执行比较，然后清理环境。
> 7. 提供卸载函数ufusr_ask_unload。
> 8. 使用UF_print_syslog函数输出日志到NX系统日志。
> 9. 使用UF_UI函数创建文件选择对话框、输出窗口等。
>
> 总体来说，这段代码实现了比较HPGL文件的完整功能，并提供了友好的用户界面。
>
