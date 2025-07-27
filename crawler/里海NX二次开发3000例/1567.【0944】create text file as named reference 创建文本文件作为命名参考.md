### 【0944】create text file as named reference 创建文本文件作为命名参考

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_ugmgr.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    #define WRITE_L(X) if (X) ECHO("%s = TRUE\n", #X); \  
        else ECHO("%s = FALSE\n", #X);  
    static void do_it(void)  
    {  
        logical  
            status = FALSE;  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            export_dir_name[MAX_FSPEC_SIZE+1],  
            encoded_name[MAX_FSPEC_SIZE+1],  
            part_file_name[UF_UGMGR_FNAME_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1];  
        FILE  
            *data_file;  
        UF_CALL(UF_UGMGR_ask_file_export_status(&status));  
        WRITE_L(status);  
        if (!UF_CALL(UF_UGMGR_ask_export_directory(part, export_dir_name)))  
        {  
            UF_CALL(UF_PART_ask_part_name(part, encoded_name));  
            UF_CALL(UF_UGMGR_decode_part_filename(encoded_name, part_number,  
                part_revision, part_file_type, part_file_name));  
            strcat(export_dir_name, "/data_file.txt");  
            data_file = fopen(export_dir_name, "w");  
            fprintf(data_file, "Text file will be stored with Item Revision%s/%s\n",  
                part_number, part_revision);  
            fclose(data_file);  
            UF_CALL(UF_PART_save());  
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

> 这段NX Open API二次开发代码主要用于实现以下功能：
>
> 1. 获取当前显示部件的信息，包括部件名称、版本号等。
> 2. 检查部件是否已导出。
> 3. 如果未导出，获取部件的导出目录，并创建一个文本文件，写入部件的编号和版本号。
> 4. 保存当前部件。
> 5. 提供错误处理机制，如果函数调用失败，会输出错误信息。
>
> 代码的关键点包括：
>
> 总体而言，这段代码实现了检查和导出当前显示部件的主要功能，并具备完善的错误处理机制。
>
