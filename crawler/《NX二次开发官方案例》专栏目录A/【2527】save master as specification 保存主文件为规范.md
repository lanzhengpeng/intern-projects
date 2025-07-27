### 【2527】save master as specification 保存主文件为规范

#### 代码

```cpp
    /*HEAD SAVE_MASTER_AS_SPECIFICATION CCC UFUN */  
    #include <stdio.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：
UF_print_syslog 是 V18 中的新增功能。 */  
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
        char  
            encoded_name[MAX_FSPEC_SIZE+1],  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_file_name[UF_UGMGR_FNAME_SIZE+1];  
        tag_t  
            part = UF_PART_ask_display_part();  
        UF_PART_ask_part_name(part, encoded_name);  
        UF_CALL(UF_UGMGR_decode_part_filename(encoded_name, part_number,  
            part_revision, part_file_type, part_file_name));  
        strcpy(part_file_type, "specification");  
        strcpy(part_file_name, "specification name");  
        UF_CALL(UF_UGMGR_encode_part_filename(part_number, part_revision,  
            part_file_type, part_file_name, encoded_name));  
        UF_CALL(UF_PART_save_as(encoded_name));       
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

> 这段NX二次开发代码主要用于保存NX部件文件，并修改其文件名和文件类型。具体功能包括：
>
> 1. 错误处理函数：report_error()，用于打印出调用UF函数出错时的错误代码和错误信息。
> 2. 主要功能函数：do_it()，用于执行以下操作：获取当前打开的NX部件的display_part。调用UF_UGMGR_decode_part_filename()，解码部件的文件名，提取部件的part number、revision、file type等信息。修改file type为specification，并设置file name为specification name。调用UF_UGMGR_encode_part_filename()，重新编码文件名。最后调用UF_PART_save_as()，以新文件名保存部件文件。
> 3. 获取当前打开的NX部件的display_part。
> 4. 调用UF_UGMGR_decode_part_filename()，解码部件的文件名，提取部件的part number、revision、file type等信息。
> 5. 修改file type为specification，并设置file name为specification name。
> 6. 调用UF_UGMGR_encode_part_filename()，重新编码文件名。
> 7. 最后调用UF_PART_save_as()，以新文件名保存部件文件。
> 8. ufusr()主函数：首先初始化NX环境，然后调用do_it()执行保存操作，最后终止NX环境。
> 9. ufusr_ask_unload()：返回立即卸载标志，表示在调用ufusr()结束后，可以立即卸载该UF程序。
>
> 总体来说，这段代码实现了在NX中打开一个部件，将其另存为新文件名和文件类型的功能，并对错误进行了处理。
>
