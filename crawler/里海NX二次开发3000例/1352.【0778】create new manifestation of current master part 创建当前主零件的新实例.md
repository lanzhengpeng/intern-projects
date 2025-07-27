### 【0778】create new manifestation of current master part 创建当前主零件的新实例

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_ugmgr.h>  
    #include <uf_assem.h>  
    #include <stdarg.h>  
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
    static void build_copy_partname(char *addon, char *copy_fspec)  
    {  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE];  
        tag_t  
            part = UF_PART_ask_display_part();  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
        UF_CALL(uc4578(part_name, 2, part_name));  
        strcat(part_name, addon);  
        UF_CALL(uc4575(dspec, 2, part_name, copy_fspec));  
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
            tceng;  
        tag_t  
            master_part = UF_PART_ask_display_part(),  
            manifestation_part,  
            instance;  
        double  
            origin[3] = {0, 0, 0},  
            matrix[6] = {1, 0, 0, 0, 1, 0};  
        char  
            master_encoded_name[MAX_FSPEC_SIZE+1],  
            manifestation_encoded_name[MAX_FSPEC_SIZE+1],  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_file_name[UF_UGMGR_FNAME_SIZE+1];  
        UF_PART_load_status_t  
            error_status;  
        UF_PART_ask_part_name(master_part, master_encoded_name);  
        UF_CALL(UF_is_ugmanager_active(&tceng));  
        if (tceng)  
        {  
            UF_CALL(UF_UGMGR_decode_part_filename(master_encoded_name, part_number,  
                part_revision, part_file_type, part_file_name));  
            strcpy(part_file_name, "man");  
            prompt_for_text("Enter name for manifestation", part_file_name);  
            UF_CALL(UF_UGMGR_encode_part_filename(part_number, part_revision,  
                "manifestation", part_file_name, manifestation_encoded_name));  
        }  
        else  
            build_copy_partname("_man", manifestation_encoded_name);  
        UF_CALL(UF_PART_new(manifestation_encoded_name, 1, &manifestation_part));  
        UF_CALL(UF_ASSEM_add_part_to_assembly(manifestation_part,  
            master_encoded_name, NULL, NULL, origin, matrix, 0, &instance,  
            &error_status));  
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

> 这段NX二次开发代码的主要功能如下：
>
> 1. 定义了一个名为ECHO的函数，用于在NX的日志窗口和系统日志中输出信息。
> 2. 定义了一个名为report_error的函数，用于在函数调用失败时报告错误信息。
> 3. 定义了一个名为build_copy_partname的函数，用于构建要创建的副本部件的文件名。
> 4. 定义了一个名为report_load_status的函数，用于报告部件加载的状态信息。
> 5. 定义了一个名为prompt_for_text的函数，用于提示用户输入文本并返回用户是否输入了文本。
> 6. 定义了一个名为do_it的主函数，用于创建当前显示部件的副本作为其表现，并将其添加到装配中。具体步骤包括获取当前显示部件的信息，构建表现部件的文件名，创建表现部件，将当前部件作为子部件添加到表现部件中，并保存表现部件。
> 7. 定义了一个名为ufusr的入口函数，用于初始化NX，调用do_it函数，然后终止NX。
> 8. 定义了一个名为ufusr_ask_unload的函数，用于在插件卸载时立即卸载。
>
> 总的来说，这段代码实现了在NX中创建当前显示部件的表现副本，并将该副本添加到装配中的功能。
>
