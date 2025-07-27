### 【1224】export selected geometry to existing part 将选定的几何图形导出到现有零件中

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
    static logical prompt_for_existing_part_name(char *prompt, char *fspec)  
    {  
        logical  
            unused;  
        int  
            resp;  
        UF_CALL(UF_UI_set_prompt(prompt));  
        if (UF_CALL(UF_UI_ask_open_part_filename(fspec, &unused, &resp))  
            || (resp == UF_UI_CANCEL)) return FALSE;  
        return TRUE;  
    }  
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            *objects;  
        char  
            part_name[MAX_FSPEC_SIZE+1] = { "" };  
        UF_PART_export_options_t  
            options = { FALSE, UF_PART_maintain_params, UF_PART_copy_exp_deeply };  
        if (((n = select_objects("Export", &objects)) > 0) &&  
            prompt_for_existing_part_name("Export Into", part_name))  
        {  
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

> 这段代码是一个NX Open的UF程序，用于实现零件的导出功能。具体来说，代码的主要功能包括：
>
> 1. 使用UF_UI_select_with_class_dialog函数弹出一个选择对话框，让用户选择要导出的零件对象。
> 2. 调用UF_PART_export_with_options函数将用户选择的零件对象导出到指定的文件中。
> 3. 在选择对话框中，用户可以选择导出的范围是整个工作部件还是仅限当前显示的部件。
> 4. 用户还可以输入要导出的零件的文件名。
> 5. 在导出时，可以选择是否保留参数，以及是否进行深度复制。
> 6. 使用自定义的错误处理函数report_error来捕捉并报告错误。
> 7. 提供了卸载函数ufusr_ask_unload。
>
> 总体来说，这段代码实现了选择零件、输入文件名、设置导出选项，并调用NX的API进行零件导出的功能。代码结构清晰，使用自定义的错误处理函数提高了程序的健壮性。
>
