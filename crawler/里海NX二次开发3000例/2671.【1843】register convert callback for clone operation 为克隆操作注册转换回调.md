### 【1843】register convert callback for clone operation 为克隆操作注册转换回调

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_clone.h>  
    #include <uf_exit.h>  
    #include <uf_ugmgr.h>  
    #include <stdarg.h>  
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
    /* qq3123197280 */  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static UF_CLONE_convert_response_t convert_part_number  
        (UF_CLONE_convert_cb_t  cb, const char *input_part_specifier, char **answer)  
    {  
        int  
            n_char;  
        char  
            *encoded,  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_file_name[UF_UGMGR_FNAME_SIZE+1],  
            clone_encoded[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_UGMGR_convert_name_from_cli(input_part_specifier, &encoded));  
        UF_CALL(UF_UGMGR_decode_part_filename(encoded, part_number,  
            part_revision, part_file_type, part_file_name));  
        strcat( part_number, "_clone" );  
        UF_CALL(UF_UGMGR_encode_part_filename(part_number, part_revision,  
            part_file_type, part_file_name, clone_encoded));  
        n_char = strlen(clone_encoded) + 1;  
        UF_CALL(allocate_memory(n_char * sizeof(char), (void **)answer));  
        strcpy(*answer, clone_encoded);  
        UF_free(encoded);  
        return UF_CLONE_use_supplied;  
    }  
    static UF_CLONE_convert_response_t convert_part_name  
        (UF_CLONE_convert_cb_t  cb, const char *input_part_specifier, char **answer)  
    {  
        int  
            n_char;  
        char  
            *encoded,  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_file_name[UF_UGMGR_FNAME_SIZE+1];  
        UF_CALL(UF_UGMGR_convert_name_from_cli(input_part_specifier, &encoded));  
        UF_CALL(UF_UGMGR_decode_part_filename(encoded, part_number,  
            part_revision, part_file_type, part_file_name));  
        n_char = strlen("cloned_from_") + strlen(part_number) + 1;  
        UF_CALL(allocate_memory(n_char * sizeof(char), (void **)answer));  
        sprintf(*answer, "cloned_from_%s", part_number);  
        UF_free(encoded);  
        return UF_CLONE_use_supplied;  
    }  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_CLONE_register_cvt_callback(UF_CLONE_part_name_convert,  
                convert_part_number, "Convert_Number_Callback", "Description", NULL, TRUE));  
        UF_CALL(UF_CLONE_register_cvt_callback(UF_CLONE_part_name_convert,  
                convert_part_name, "Convert_Name_Callback", "Description", NULL, TRUE));  
        UF_terminate();  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的NX二次开发代码，主要功能包括：
>
> 1. 错误报告和内存分配：定义了ECHO函数用于打印错误信息，以及allocate_memory函数用于分配内存。
> 2. 零件编号转换：定义了convert_part_number函数，用于在零件克隆时，将输入的零件标识符转换为克隆零件的编号。
> 3. 零件名称转换：定义了convert_part_name函数，用于在零件克隆时，将输入的零件标识符转换为克隆零件的名称。
> 4. UFSTA函数：该函数用于初始化NX，注册零件编号和名称转换的回调函数，然后在结束时关闭NX。
>
> 通过这段代码，用户可以为NX添加自定义的零件克隆编号和名称转换功能，从而在克隆零件时生成符合特定要求的编号和名称。这增强了NX零件克隆功能的灵活性。
>
