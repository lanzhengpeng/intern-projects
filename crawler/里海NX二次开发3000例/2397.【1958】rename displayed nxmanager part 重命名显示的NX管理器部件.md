### 【1958】rename displayed nxmanager part 重命名显示的NX管理器部件

#### 代码

```cpp
    /*HEAD RENAME_DISPLAYED_UGMGR_PART CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_part.h>  
    #include <uf_ugmgr.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据注释中的内容，UF_print_syslog 是 V18 新增的功能。因此，我的回答是：

UF_print_syslog 是 V18 中新增的功能。 */  
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
            ii;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            other = part;  
        char  
            part_fspec[MAX_FSPEC_SIZE+1],  
            encoded_name[MAX_FSPEC_SIZE+1],  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_file_name[UF_UGMGR_FNAME_SIZE+1];  
        UF_PART_ask_part_name( part, part_fspec );  
        UF_CALL( UF_UGMGR_decode_part_filename( part_fspec, part_number,  
                                                part_revision,part_file_type,  
                                                part_file_name ));  
        strcat( part_number, "_new" );  
        UF_CALL( UF_UGMGR_encode_part_filename( part_number, part_revision,  
                                                part_file_type, part_file_name,  
                                                encoded_name ));                                  
        UF_CALL( UF_PART_rename( part, encoded_name ));  
    /*  Work around to update ANT and Displayed Part in Graphics window 里海译:绕过更新ANT和图形窗口中显示的部件的方法。 */  
        for (ii = 0; ((ii < UF_PART_ask_num_parts()) && (other == part)); ii++)  
            other = UF_PART_ask_nth_part(ii);  
        UF_CALL(UF_PART_set_display_part(other));  
        UF_CALL(UF_PART_set_display_part(part));  
    /*  End work around 里海译:End work around，翻译为结束工作回避。 */  
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

> 这段代码是一个NX二次开发示例，主要实现了以下功能：
>
> 1. 包含了必要的NX Open头文件，如uf.h、uf_part.h等。
> 2. 定义了一个报告错误函数report_error，用于在函数调用出错时打印错误信息。
> 3. 定义了主函数do_it，实现了以下功能：获取当前显示的部件对象。获取部件的名称、编号、版本等属性。在编号后面添加"_new"后缀，生成新的部件名称。重命名当前显示的部件。更新图形窗口中显示的部件，以反映名称变更。
> 4. 获取当前显示的部件对象。
> 5. 获取部件的名称、编号、版本等属性。
> 6. 在编号后面添加"_new"后缀，生成新的部件名称。
> 7. 重命名当前显示的部件。
> 8. 更新图形窗口中显示的部件，以反映名称变更。
> 9. 定义了ufusr函数，作为NX的回调函数，调用do_it函数来实现上述功能。
> 10. 定义了ufusr_ask_unload函数，用于卸载用户函数模块。
>
> 整体而言，这段代码实现了在NX中重命名当前显示的部件，并更新图形窗口中的显示，以反映名称变更的功能。代码结构清晰，提供了错误处理和必要的注释，是一个NX二次开发的好示例。
>
