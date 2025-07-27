### 【1698】open close speed test 开关速度测试

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
    static logical prompt_for_an_integer(char *prompt, char *item, int *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        int  
            da[1];  
        strncpy(&menu[0][0], item, 15);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
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
    #define WRITE_F(X) ECHO("%s = %f\n", #X, X)  
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
            ii,  
            n = 50;  
        tag_t  
            part = NULL_TAG;  
        char  
            fspec[MAX_FSPEC_SIZE+1] = { "" };  
        UF_timer_t  
            timer;  
        UF_timer_values_t  
            times;  
        UF_PART_load_status_t  
            status;  
        while ((prompt_for_existing_part_name("Open/Close speed test", fspec))  
            && prompt_for_an_integer("How many?", "no. times", &n))  
        {  
            UF_CALL(UF_begin_timer(&timer));  
            for (ii = 0; ii < n; ii++)  
            {  
                UF_CALL(UF_PART_open_quiet(fspec, &part, &status));  
                if (status.n_parts > 0) report_load_status(&status);  
                if (part == NULL_TAG) break;  
                UF_CALL(UF_PART_close(part, 1, 1));  
            }  
            UF_CALL(UF_end_timer(timer, &times));  
            WRITE_F(times.cpu_time);  
            WRITE_F(times.real_time);  
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

> 这段代码是一个NX Open API的二次开发示例，主要实现了以下功能：
>
> 1. 定义了错误报告函数report_error，用于在函数调用失败时报告错误信息。
> 2. 定义了提示输入整数和提示选择现有部件名称的函数prompt_for_an_integer和prompt_for_existing_part_name，用于交互式输入。
> 3. 定义了报告加载状态的函数report_load_status，用于报告部件打开时的状态信息。
> 4. 定义了主函数do_it，用于打开和关闭指定次数的部件，并记录耗时。
> 5. 定义了ufusr函数，作为程序的入口，初始化并调用do_it函数，然后终止。
> 6. 定义了ufusr_ask_unload函数，用于卸载插件。
>
> 整体而言，这段代码通过交互式输入获取打开次数和部件名称，然后循环打开和关闭指定次数的部件，并记录耗时。如果打开失败，会报告错误信息。最后，提供了卸载插件的接口。
>
