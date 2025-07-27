### 【1713】open selected part 打开选定部件

#### 代码

```cpp
    /*HEAD OPEN_SELECTED_PART CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_exit.h>  
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
    static void do_it(void)  
    {  
        int  
            resp;  
        logical  
            disp = FALSE;  
        tag_t  
            part;  
        char  
            file_name[MAX_FSPEC_SIZE + 1] = { "" };  
        UF_PART_load_status_t  
            status;  
        if (!(UF_CALL(UF_UI_ask_open_part_filename(file_name, &disp, &resp))) &&  
             (resp == UF_UI_OK) &&  
             (UF_CALL(UF_PART_open(file_name, &part, &status)))  
             || status.n_parts > 0)  
            report_load_status(&status);  
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

> 这段代码是一个NX Open的二次开发示例，实现了以下功能：
>
> 1. 包含了必要的NX Open头文件，包括UF、UF_UI、UF_PART等。
> 2. 定义了一个ECHO函数，用于打印信息到NX的日志窗口和系统日志。
> 3. 定义了一个report_error函数，用于在函数调用失败时报告错误信息。
> 4. 定义了一个report_load_status函数，用于报告加载部件的状态信息。
> 5. 定义了一个do_it函数，该函数实现了以下逻辑：调用UF_UI_ask_open_part_filename函数，提示用户打开一个部件文件。如果用户选择了一个文件并确认，调用UF_PART_open函数加载该部件文件。如果加载成功，调用report_load_status函数报告加载状态。
> 6. 调用UF_UI_ask_open_part_filename函数，提示用户打开一个部件文件。
> 7. 如果用户选择了一个文件并确认，调用UF_PART_open函数加载该部件文件。
> 8. 如果加载成功，调用report_load_status函数报告加载状态。
> 9. 定义了ufusr函数，这是NX Open程序的入口函数。在该函数中，先调用UF_initialize初始化NX Open，然后调用do_it执行主逻辑，最后调用UF_terminate终止NX Open。
> 10. 定义了ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY，表示当用户退出程序时立即卸载NX Open。
>
> 总体来说，这段代码通过UI交互让用户选择一个部件文件并加载，加载后报告状态信息，是一个典型的NX Open二次开发示例。
>
