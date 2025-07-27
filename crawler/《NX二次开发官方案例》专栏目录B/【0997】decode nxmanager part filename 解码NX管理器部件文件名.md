### 【0997】decode nxmanager part filename 解码NX管理器部件文件名

#### 代码

```cpp
    /*HEAD DECODE_UGMGR_PART_FILENAME CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据上下文，UF_print_syslog 是 V18 版本中新增的功能。 */  
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
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            cli_name[MAX_FSPEC_SIZE+1],  
            encoded_name[MAX_FSPEC_SIZE+1],  
            part_file_name[UF_UGMGR_FNAME_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1];  
        UF_PART_ask_part_name(part, encoded_name);  
        WRITE_S(encoded_name);  
        UF_CALL(UF_UGMGR_decode_part_filename(encoded_name, part_number,  
            part_revision, part_file_type, part_file_name));  
        WRITE_S(part_number);  
        WRITE_S(part_revision);  
        WRITE_S(part_file_type);  
        WRITE_S(part_file_name);  
        UF_CALL(UF_UGMGR_convert_name_to_cli(encoded_name, cli_name));  
        WRITE_S(cli_name);  
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

> 这段代码是NX的二次开发代码，主要功能是从NX部件的文件名中解码出部件的编号、版本号、文件类型和文件名。
>
> 主要内容包括：
>
> 1. 定义了错误报告函数report_error，用于在函数调用出错时输出错误信息。
> 2. 定义了写入字符串到列表窗口的函数write_string_to_listing_window。
> 3. 定义了主函数do_it：
>
> 1. 定义了ufusr函数，用于初始化NX环境并调用do_it函数，然后终止NX环境。
> 2. 定义了卸载函数ufusr_ask_unload。
>
> 总体来说，这段代码实现了从NX部件的文件名中解码出部件信息的二次开发功能。
>
