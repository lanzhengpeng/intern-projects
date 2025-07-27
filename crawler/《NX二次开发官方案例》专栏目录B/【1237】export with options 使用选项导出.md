### 【1237】export with options 使用选项导出

#### 代码

```cpp
    /*HEAD EXPORT_WITH_OPTIONS CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中的新增功能，用于打印系统日志信息。 */  
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
    static int select_objects(char *prompt, tag_t **objects)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select Objects", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, NULL, NULL, &resp, &cnt, objects));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*objects)[ii], 0));  
        return cnt;  
    }  
    static void build_temp_partname(char *copy_fspec)  
    {  
        logical  
            tceng;  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            *tmp_dir;  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(UF_is_ugmanager_active(&tceng));  
        if (tceng)  
        {  
            UF_CALL(UF_UGMGR_decode_part_filename(part_fspec, part_name, dspec,  
                dspec, dspec));  
        }  
        else  
        {  
            UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
            UF_CALL(uc4578(part_name, 2, part_name));  
        }  
        strcat(part_name, "-tmp");  
        UF_CALL(uc4575(tmp_dir, 2, part_name, copy_fspec));  
    }  
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            *objects;  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_export_options_t  
            options = { TRUE, UF_PART_maintain_params, UF_PART_copy_exp_deeply };  
        if ((n = select_objects("export", &objects)) > 0)  
        {  
            build_temp_partname(part_name);  
            UF_CALL(UF_PART_export_with_options( part_name, n, objects, &options));  
            UF_free(objects);  
        }  
    }  
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

> 这段代码是NX的二次开发示例代码，其主要功能是实现了以下功能：
>
> 1. 使用UF_UI_select_with_class_dialog函数，实现了选择工作部件中的对象的功能。
> 2. 使用UF_PART_export_with_options函数，实现了将选择的对象导出到临时部件的功能。
> 3. 定义了一个错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 4. 定义了一个构建临时部件名称的函数build_temp_partname。
> 5. 定义了一个执行导出操作的函数do_it。
> 6. 在ufusr函数中，初始化NX环境，调用do_it函数执行导出操作，最后终止NX环境。
> 7. 定义了一个卸载函数ufusr_ask_unload，用于卸载用户自定义函数。
>
> 总的来说，这段代码通过NX提供的API函数实现了选择对象并导出到临时部件的功能。
>
