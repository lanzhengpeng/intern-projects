### 【2061】report description of specified parts 报告指定部件的描述

#### 代码

```cpp
    /*HEAD REPORT_DESCRIPTION_OF_SPECIFIED_PARTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog是在V18版本中新增的。 */  
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
    static logical prompt_for_part_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[MAX_FSPEC_SIZE+1],  
            *p_dir,  
            *p_ext;  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_PART_DIR, &p_dir));  
        UF_CALL(UF_UI_ask_dialog_filter(UF_UI_PART_OPEN_FLTR, &p_ext));  
        combine_directory_and_wildcard(p_dir, p_ext, filter);  
        UF_free(p_dir);  
        UF_free(p_ext);  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "Part Name", filter, "", fspec,  
            &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
    }  
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            UF_UI_open_listing_window();  
            UF_UI_write_listing_window("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_UI_write_listing_window("  ");  
                UF_UI_write_listing_window(status->file_names[ii]);  
                UF_UI_write_listing_window(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window("\n");  
            }  
            UF_UI_write_listing_window("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
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
    static void do_it(void)  
    {  
        int  
            was_loaded;  
        tag_t  
            part;  
        char  
            *description,  
            part_spec[MAX_FSPEC_SIZE+1] = { "" };  
        UF_PART_load_status_t  
            error_status;  
        while (prompt_for_part_name("Report description", part_spec))  
        {  
            WRITE_S(part_spec);  
            was_loaded = UF_PART_is_loaded(part_spec);  
            if (!was_loaded)  
            {  
                UF_CALL(UF_PART_open_quiet(part_spec, &part, &error_status));  
                if (error_status.n_parts > 0) report_load_status(&error_status);  
            }  
            else  
            {  
                UF_CALL(UF_PART_ask_tag_of_disp_name(part_spec, &part));  
            }  
            if (!UF_CALL(UF_PART_ask_description(part, &description)) &&  
                (description != NULL))  
            {  
                WRITE_S(description);  
                UF_free(description);  
            }  
            if (!was_loaded) UF_CALL(UF_PART_close(part, 0, 1));  
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

> 这段代码是用于NX的二次开发，主要实现了以下几个功能：
>
> 1. 错误报告：通过定义UF_CALL宏，可以调用NX API函数，并自动检查返回码。如果返回错误码，则会打印错误信息到系统日志和对话框。
> 2. 文件选择对话框：通过调用NX的UI函数，可以弹出一个文件选择对话框，用于选择要报告描述的部件文件。
> 3. 部件加载状态报告：可以打开一个部件文件，并报告加载状态，包括每个部件的文件名和加载状态信息。
> 4. 描述信息输出：获取指定部件的描述信息，并输出到对话框。
> 5. 资源释放：在打开部件文件后，会释放相关资源。
> 6. 初始化和终止：在ufusr函数中，进行NX的初始化和终止操作。
> 7. 卸载请求：定义了ufusr_ask_unload函数，用于在卸载用户程序时立即卸载。
>
> 总体而言，这段代码实现了选择部件、打开部件、报告部件描述信息的完整流程，并且具有完善的错误处理和资源管理。
>
