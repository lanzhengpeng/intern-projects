### 【2751】set displayed part from non-master to master 从非主模型切换到主模型

#### 代码

```cpp
    /*HEAD SET_DISPLAYED_PART_FROM_NON-MASTER_TO_MASTER CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog is new in V18。 */  
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
    static void do_it(void)  
    {  
        char  
            encoded_name[MAX_FSPEC_SIZE+1],  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_file_name[UF_UGMGR_FNAME_SIZE+1];  
        tag_t  
            part = UF_PART_ask_display_part(),  
            master;  
        UF_PART_load_status_t  
            error_status;      
        UF_PART_ask_part_name(part, encoded_name);  
        UF_CALL(UF_UGMGR_decode_part_filename(encoded_name, part_number,  
            part_revision, part_file_type, part_file_name));  
        strcpy(part_file_type, "master");  
        strcpy(part_file_name, "");  
        UF_CALL(UF_UGMGR_encode_part_filename(part_number, part_revision,  
            part_file_type, part_file_name, encoded_name));  
        if ((UF_PART_is_loaded(encoded_name)) != 1)  
        {  
            UF_CALL(UF_PART_open_quiet(encoded_name, &master, &error_status));  
            if (error_status.n_parts > 0) report_load_status(&error_status);  
        }  
        else      
            master = UF_PART_ask_part_tag(encoded_name);  
        UF_CALL(UF_PART_set_display_part(master));      
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

> 这段NX二次开发代码的主要功能是从非主模型切换到主模型，并实现以下关键功能：
>
> 1. 包含必要的头文件，以使用NX提供的函数库。
> 2. 定义了一个错误报告函数report_error，用于在调用NX函数失败时打印错误信息。
> 3. 定义了一个报告加载状态函数report_load_status，用于打印加载零件时出现的错误信息。
> 4. 定义了主函数do_it，其步骤包括：获取当前显示的零件信息解析出零件的编号、版本等重新编码出主模型零件的文件名如果主模型尚未加载，则加载主模型设置当前显示的零件为主模型
> 5. 获取当前显示的零件信息
> 6. 解析出零件的编号、版本等
> 7. 重新编码出主模型零件的文件名
> 8. 如果主模型尚未加载，则加载主模型
> 9. 设置当前显示的零件为主模型
> 10. 定义了ufusr函数作为NX二次开发入口，初始化后调用do_it函数，最后终止NX。
> 11. 定义了ufusr_ask_unload函数，返回立即卸载标志，用于在退出二次开发应用时卸载加载的零件。
>
> 总体来说，这段代码实现了从非主模型切换到主模型的功能，包括错误处理和加载状态的报告。
>
