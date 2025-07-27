### 【1711】open selected jt file 打开选定的JT文件

#### 代码

```cpp
    /*HEAD OPEN_SELECTED_JT_FILE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n*** ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据注释，UF_print_syslog 是 V18 版本中新增的功能，用于打印系统日志。 */  
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
    static logical prompt_for_jt_file_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[MAX_FSPEC_SIZE+1],  
            *p_dir;  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_PART_DIR, &p_dir));  
        combine_directory_and_wildcard(p_dir, "*.jt", filter);  
        UF_free(p_dir);  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "JT File Name", filter, "",  
            fspec, &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
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
        tag_t  
            part;  
        char  
            file_name[UF_CFI_MAX_PATH_NAME_SIZE] = { "" };  
        UF_PART_load_status_t  
            status;  
        if (prompt_for_jt_file_name("Open", file_name))  
        {  
             UF_CALL(UF_PART_open(file_name, &part, &status));  
             if (status.n_parts > 0) report_load_status(&status);  
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

> 这段代码是NX二次开发的示例，其主要功能是打开一个JT文件。下面是代码的主要功能：
>
> 1. 错误报告函数：该函数用于在调用NX API函数出错时，打印错误信息到系统日志和对话框中。
> 2. 组合目录和通配符函数：该函数将目录路径和文件通配符组合成一个完整的文件路径。
> 3. 提示输入JT文件名函数：该函数会弹出一个对话框，提示用户输入JT文件名，并返回文件路径。
> 4. 报告加载状态函数：该函数用于打印出打开JT文件时各组件的加载状态信息。
> 5. 执行打开文件函数：该函数会提示用户输入JT文件名，并调用NX API函数打开该文件，打印出加载状态。
> 6. UFusr函数：这是NX二次开发的主函数，用于初始化NX，调用执行打开文件的函数，然后终止NX。
> 7. 卸载询问函数：该函数用于返回立即卸载标志。
>
> 总体来说，该代码通过二次开发的方式，实现了打开用户指定的JT文件的功能，并在出错时提供了友好的错误信息提示。
>
