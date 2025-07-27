### 【1236】export with options export with options

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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据注释，UF_print_syslog 是在 V18 版本中新增的功能，用于打印系统日志。因此，我的翻译是：UF_print_syslog 是在 V18 版本中新增的功能，用于打印系统日志。 */  
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

> 这段代码是一个NX的二次开发代码，主要功能是导出选中的对象。下面是代码的主要功能和步骤：
>
> 1. 引入必要的NX API头文件，如uf.h、uf_ui.h等。
> 2. 定义宏UF_CALL，用于调用NX API函数，并在出错时报告错误。
> 3. 定义report_error函数，用于报告错误信息。
> 4. 定义select_objects函数，用于弹出对话框让用户选择要导出的对象，并返回对象数组。
> 5. 定义build_temp_partname函数，用于构建临时部件文件名。
> 6. 定义do_it函数，首先调用select_objects获取对象数组，然后构建临时文件名，最后调用UF_PART_export_with_options函数导出对象到临时文件。
> 7. 定义ufusr函数，这是用户自定义函数，在NX启动时调用。在这个函数中，初始化NX环境，调用do_it执行导出，然后终止NX环境。
> 8. 定义ufusr_ask_unload函数，用于卸载NX时立即卸载二次开发应用程序。
>
> 总的来说，这段代码通过二次开发实现了NX中选中对象的导出功能，具有错误处理和用户交互等功能。
>
