### 【1695】open and save specified part 打开并保存指定的部件

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
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
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
             (resp == UF_UI_OK))  
        {  
            UF_CALL(UF_PART_open(file_name, &part, &status));  
            if (status.n_parts > 0) report_load_status(&status);  
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

> 这段代码是一段NX二次开发代码，主要包括以下功能：
>
> 1. 定义了一个ECHO宏，用于在列表窗口和系统日志中输出信息。
> 2. 定义了一个UF_CALL宏，用于调用UF函数，并捕获错误，输出错误信息。
> 3. 定义了一个report_error函数，用于输出UF函数调用的错误信息。
> 4. 定义了一个report_load_status函数，用于输出零件加载状态信息。
> 5. 定义了一个do_it函数，用于执行主要功能：提示用户打开一个NX零件文件，然后加载该文件，并保存当前打开的零件。
> 6. 定义了ufusr函数，作为NX二次开发的入口函数，用于初始化和终止NX，并调用do_it函数。
> 7. 定义了ufusr_ask_unload函数，用于卸载当前加载的零件。
>
> 总体来说，这段代码实现了在NX中打开一个零件文件，然后保存当前打开的零件的功能。
>
