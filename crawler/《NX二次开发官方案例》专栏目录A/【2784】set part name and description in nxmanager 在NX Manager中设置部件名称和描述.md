### 【2784】set part name and description in nxmanager 在NX Manager中设置部件名称和描述

#### 代码

```cpp
    /*HEAD SET_PART_NAME_AND_DESC_IN_UGMGR CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog是V18版本新增的功能。因此，可以简洁地翻译为：

UF_print_syslog是V18版本新增的功能。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
         tag_t  
            part = UF_PART_ask_display_part();  
         UF_UGMGR_tag_t  
            db_tag;  
         char  
            part_file_name[UF_UGMGR_FNAME_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            part_desc[UF_UGMGR_DESC_SIZE+1],  
            encoded_name[UF_CFI_MAX_PATH_NAME_SIZE];  
         UF_PART_ask_part_name(part, encoded_name);  
         UF_CALL(UF_UGMGR_decode_part_filename(encoded_name, part_number,  
            part_revision, part_file_type, part_file_name));  
         UF_CALL(UF_UGMGR_ask_part_tag(part_number, &db_tag ));  
         WRITE_D(db_tag);  
         WRITE("Before UF_UGMGR_set_part_name_desc:\n");  
         UF_CALL(UF_UGMGR_ask_part_name_desc(db_tag, part_file_name, part_desc));  
         WRITE_S(part_file_name);  
         WRITE_S(part_desc);  
         UF_CALL(UF_UGMGR_set_part_name_desc(db_tag,"new_name","new_description"));  
         WRITE("After UF_UGMGR_set_part_name_desc:\n");  
         UF_CALL(UF_UGMGR_ask_part_name_desc(db_tag, part_file_name, part_desc));  
         WRITE_S(part_file_name);  
         WRITE_S(part_desc);  
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

> 根据代码内容，这是一段基于NX的二次开发代码，主要功能是设置UG管理器中部件的名称和描述。代码的主要流程包括：
>
> 1. 包含必要的NX头文件，如uf.h, uf_ui.h, uf_part.h, uf_cfi.h, uf_ugmgr.h等。
> 2. 定义宏UF_CALL，用于调用NX API函数，并报告错误。定义宏WRITE用于输出信息。
> 3. 定义函数report_error，用于输出错误信息。
> 4. 定义函数write_integer_to_listing_window和write_string_to_listing_window，用于输出整型和字符串信息。
> 5. 定义函数do_it，其中获取当前显示的部件，解码其名称，获取部件的标签，输出部件的文件名和描述，然后调用UF_UGMGR_set_part_name_desc设置新的名称和描述，并再次输出信息。
> 6. 定义ufusr函数，用于初始化NX环境，调用do_it函数，然后终止NX环境。
> 7. 定义ufusr_ask_unload函数，用于卸载插件。
>
> 总的来说，这段代码的主要功能是设置NX UG管理器中部件的名称和描述，包括获取当前部件信息，设置新名称和描述，并输出相关信息。代码结构清晰，注释详细，便于理解和维护。
>
