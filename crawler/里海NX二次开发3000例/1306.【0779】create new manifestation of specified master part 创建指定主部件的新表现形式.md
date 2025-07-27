### 【0779】create new manifestation of specified master part 创建指定主部件的新表现形式

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_ugmgr.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
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
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        int  
            ii;  
        char  
            msg[133];  
        if (status->n_parts > 0)  
        {  
            ECHO("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_get_fail_message(status->statuses[ii], msg);  
                ECHO("  %s - %s\n", status->file_names[ii], msg);  
            }  
            ECHO("\n");  
            UF_CALL(UF_PART_free_load_status(status));  
        }  
    }  
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
        logical  
            unused;  
        int  
            resp;  
        tag_t  
            instance,  
            part;  
        double  
            origin[3] = {0, 0, 0},  
            matrix[6] = {1, 0, 0, 0, 1, 0};  
        char  
            master_encoded_name[MAX_FSPEC_BUFSIZE ] = { "" },  
            part_number[UF_UGMGR_PARTNO_BUFSIZE] ,  
            part_revision[UF_UGMGR_PARTREV_BUFSIZE] ,  
            part_file_type[UF_UGMGR_FTYPE_BUFSIZE] ,  
            part_file_name[UF_UGMGR_FNAME_BUFSIZE],  
            manifestation_encoded_name[MAX_FSPEC_BUFSIZE ];  
        UF_PART_load_status_t  
            error_status;  
        if (UF_CALL(UF_UI_ask_open_part_filename(master_encoded_name, &unused,  
            &resp)) || (resp != UF_UI_OK)) return;  
        UF_CALL(UF_UGMGR_decode_part_file_name(master_encoded_name, part_number,  
            part_revision, part_file_type, part_file_name));  
        strcpy(part_file_name, "man");  
        if (!prompt_for_text("Enter Name for new manifestation", part_file_name))  
            return;  
        UF_CALL(UF_UGMGR_encode_part_filename(part_number, part_revision,  
            "manifestation", part_file_name, manifestation_encoded_name));  
        UF_CALL(UF_PART_new(manifestation_encoded_name, UF_PART_ENGLISH, &part));  
        UF_CALL(UF_ASSEM_add_part_to_assembly(part, master_encoded_name, NULL, NULL,  
            origin, matrix, 0, &instance, &error_status));  
        if (error_status.n_parts > 0) report_load_status(&error_status);  
        UF_CALL(UF_PART_save());  
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

> 这段代码是一个NX二次开发的示例，主要功能是创建一个新的零件实例，并添加到装配中。以下是代码的主要步骤：
>
> 1. 定义了一些宏和辅助函数，用于错误报告和字符串格式化。
> 2. do_it函数用于实现主要功能：通过UF_UI_ask_open_part_filename函数获取用户选择的零件文件名。使用UF_UGMGR_decode_part_file_name解析零件信息，包括零件号、版本、类型和文件名。修改文件名，并提示用户输入新实例的名称。使用UF_UGMGR_encode_part_filename编码新实例的零件文件名。调用UF_PART_new创建新实例的零件对象。使用UF_ASSEM_add_part_to_assembly将新实例添加到装配中。如果有加载错误，则报告错误。保存零件。
> 3. 通过UF_UI_ask_open_part_filename函数获取用户选择的零件文件名。
> 4. 使用UF_UGMGR_decode_part_file_name解析零件信息，包括零件号、版本、类型和文件名。
> 5. 修改文件名，并提示用户输入新实例的名称。
> 6. 使用UF_UGMGR_encode_part_filename编码新实例的零件文件名。
> 7. 调用UF_PART_new创建新实例的零件对象。
> 8. 使用UF_ASSEM_add_part_to_assembly将新实例添加到装配中。
> 9. 如果有加载错误，则报告错误。
> 10. 保存零件。
> 11. ufusr函数是二次开发的入口函数，调用UF_initialize初始化NX环境，调用do_it函数执行主要功能，然后调用UF_terminate终止NX环境。
> 12. ufusr_ask_unload函数用于设置二次开发模块卸载时的行为，这里设置为立即卸载。
>
> 综上所述，这段代码实现了在NX中创建新零件实例并添加到装配中的功能，包含了错误处理、用户交互等逻辑。
>
