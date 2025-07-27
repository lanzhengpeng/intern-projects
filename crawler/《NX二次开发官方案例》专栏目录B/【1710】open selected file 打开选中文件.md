### 【1710】open selected file 打开选中文件

#### 代码

```cpp
    /*HEAD OPEN_SELECTED_FILE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
    #include <uf_ugmgr.h>  
    #include <uf_exit.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 版本中的新增功能。 */  
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
    static void build_similar_temp_filespec(char *fspec, int ftype, char *new_spec)  
    {  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            temp_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            *tmp_dir;  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_CALL(uc4576(fspec, 0, dspec, temp_name));  
        if (strrchr(temp_name, '.')) strcpy(strrchr(temp_name, '.'), "");  
        UF_CALL(uc4575(tmp_dir, ftype, temp_name, new_spec));  
    }  
    static void change_filespec_extension(char *fspec, int ftype, char *new_fspec)  
    {  
        logical  
            is_ugmgr;  
        int  
            orig_ftype;  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            shrt_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_file_name[UF_UGMGR_FNAME_SIZE+1];  
        UF_CALL(UF_is_ugmanager_active(&is_ugmgr));  
        UF_CALL(uc4564(fspec, 0, (1 << 4))); /* fmode cannot be 0 里海译:fmode不能为0。 */  
        orig_ftype = uc4601();  
        if (!is_ugmgr)  
        {  
            UF_CALL(uc4576(fspec, orig_ftype, dspec, shrt_name));  
            UF_CALL(uc4578(shrt_name, orig_ftype, shrt_name));  
            UF_CALL(uc4575(dspec, ftype, shrt_name, new_fspec));  
        }  
        else  
        {  
            UF_CALL(UF_UGMGR_decode_part_filename(fspec, part_number,  
                part_revision, part_file_type, part_file_name));  
            build_similar_temp_filespec(part_number, ftype, new_fspec);  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            ftype,  
            resp;  
        logical  
            disp = FALSE;  
        tag_t  
            part;  
        char  
            *base_dir,  
            cmd[UF_UI_MAX_STRING_LEN+1],  
            def_file[UF_CFI_MAX_PATH_NAME_SIZE],  
            file_name[UF_CFI_MAX_PATH_NAME_SIZE] = { "" },  
            part_name[UF_CFI_MAX_PATH_NAME_SIZE],  
            prog_name[UF_CFI_MAX_PATH_NAME_SIZE],  
            step_dir[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_PART_load_status_t  
            status;  
        if (!(UF_CALL(UF_UI_ask_open_part_filename(file_name, &disp, &resp))) &&  
             (resp == UF_UI_OK))  
        {  
        /*  The following is a work around for PR 4975151 里海译:以下是对PR 4975151的解决方法。 */  
            UF_CALL(uc4564(file_name, 0, (1 << 4))); /* fmode cannot be 0 里海译:fmode cannot be 0的翻译为：fmode不能为0。 */  
            ftype = uc4601();  
            if (ftype == 33) /* step203 里海译:Step 203: Deploying the application to the server */  
            {  
                change_filespec_extension(file_name, 2, part_name);  
                UF_CALL(UF_translate_variable("UGII_BASE_DIR", &base_dir));  
                UF_CALL(uc4575(base_dir, 100, "step203ug", step_dir));  
                UF_CALL(uc4575(step_dir, 0, "step203ug", prog_name));  
                UF_CALL(uc4575(step_dir, 0, "step203ug.def", def_file));  
    #if defined(_WIN32)  
                strcat(prog_name, ".cmd");  
                sprintf(cmd, "\"\"%s\" D=\"%s\" O=\"%s\" \"%s\"\"",  
                    prog_name, def_file, part_name, file_name);  
    #else  
                sprintf(cmd, "%s D=%s O=%s %s",  
                    prog_name, def_file, part_name, file_name);  
    #endif  
                system(cmd);  
                if (UF_CALL(UF_PART_open(part_name, &part, &status))  
                    || status.n_parts > 0) report_load_status(&status);  
            }  
        /*  End of work around for PR 4975151 里海译:对于PR 4975151的绕行工作结束。 */  
            else  
            {  
                if (UF_CALL(UF_PART_open(file_name, &part, &status))  
                    || status.n_parts > 0) report_load_status(&status);  
            }  
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

> 这是段NX（Siemens NX）的二次开发代码，其主要功能是打开一个NX部件文件，并根据文件类型执行不同的操作。
>
> 代码的关键点包括：
>
> 1. 使用UF_PART_open函数打开指定的NX部件文件，并获取加载状态信息。
> 2. 若文件类型为STEP203格式，则调用系统命令执行step203ug程序，将STEP203文件转换为NX部件文件。这一步是针对PR 4975151的解决方法。
> 3. 若文件类型不是STEP203格式，则直接打开文件，并报告加载状态。
> 4. 定义了一个错误报告函数report_error，用于输出错误信息。
> 5. 定义了一个加载状态报告函数report_load_status，用于输出加载状态信息。
> 6. 定义了两个文件名处理函数build_similar_temp_filespec和change_filespec_extension，用于生成临时文件名和修改文件扩展名。
> 7. 在ufusr函数中初始化和终止NX环境，并调用do_it函数执行文件打开操作。
> 8. 定义了ufusr_ask_unload函数，返回立即卸载标志，用于卸载二次开发环境。
>
> 总体来说，这段代码实现了打开NX部件文件，并根据文件类型执行特定操作的功能。
>
