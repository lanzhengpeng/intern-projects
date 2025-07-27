### 【1689】open and close parts listed in text file 打开文本文件中列出的部件，然后关闭它们

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_ugmgr.h>  
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
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int ask_data_from_text_file(char *fspec, char ***data)  
    {  
        int  
            ii = 0,  
            n_lines = 0;  
        char  
            a_line[MAX_LINE_SIZE+1];  
        FILE  
            *the_file;  
        if ((the_file = fopen(fspec, "r")) != NULL)  
        {  
            while ((fgets(a_line, MAX_LINE_SIZE, the_file)) != NULL) n_lines++;  
            rewind(the_file);  
            UF_CALL(allocate_memory(n_lines * sizeof(char *), (void **)data));  
            for (ii = 0; ii < n_lines; ii++)  
            {  
                fgets(a_line, MAX_LINE_SIZE, the_file);  
            /*  strip off the carriage return 里海译:剥离回车 */  
                a_line[strlen(a_line) - 1] = '\0';  
                UF_CALL(allocate_memory((unsigned int)(strlen(a_line) + 1),  
                    (void **)&(*data)[ii]));  
                strcpy((*data)[ii], a_line);  
            }  
            fclose(the_file);  
        }  
        return n_lines;  
    }  
    static logical prompt_for_text_file_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[MAX_FSPEC_SIZE+1] = { "*.txt" };  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "Text File Name", filter, "",  
            fspec, &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
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
    static logical ask_yes_or_no(char *title, char *message)  
    {  
        int  
            resp;  
        UF_UI_message_buttons_t  
            buttons = { TRUE, FALSE, TRUE, "Yes", NULL, "No", 1, 0, 2 };  
        UF_CALL(UF_UI_message_dialog(title, UF_UI_MESSAGE_QUESTION, &message, 1,  
            TRUE, &buttons, &resp));  
        return (2 - resp);  
    }  
    static void do_it(void)  
    {  
        logical  
            nxman;  
        int  
            ii,  
            n;  
        tag_t  
            part;  
        char  
            **part_names,  
            prompts[2][31] = { "List of part path names", "List of cli names" },  
            tfn[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_PART_load_status_t  
            status;  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        if (!prompt_for_text_file_name(prompts[nxman], tfn)) return;  
        n = ask_data_from_text_file(tfn, &part_names);  
        for (ii = 0; ii < n; ii++)  
        {  
            ECHO("%s\n", part_names[ii]);  
            UF_CALL(UF_PART_open(part_names[ii], &part, &status));  
            if (status.n_parts > 0) report_load_status(&status);  
            if (!ask_yes_or_no("Next", "Continue?")) break;  
            UF_CALL(UF_PART_close_all());  
        }  
        UF_free_string_array(n, part_names);  
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

> 根据代码的注释和函数名，这是一段用于在NX中进行二次开发的代码，其主要功能包括：
>
> 1. 定义了ECHO函数，用于在窗口和系统日志中输出信息。
> 2. 定义了UF_CALL宏，用于调用NX的API函数，并在出错时报告错误信息。
> 3. 定义了allocate_memory函数，用于分配内存。
> 4. 定义了ask_data_from_text_file函数，用于从文本文件中读取数据。
> 5. 定义了prompt_for_text_file_name函数，用于提示用户输入文本文件名。
> 6. 定义了report_load_status函数，用于报告零件加载状态。
> 7. 定义了ask_yes_or_no函数，用于询问用户是否继续。
> 8. 定义了do_it函数，用于执行打开零件、询问是否继续等操作。
> 9. 定义了ufusr函数，这是NX二次开发的入口函数，在其中调用do_it函数。
> 10. 定义了ufusr_ask_unload函数，用于卸载二次开发。
>
> 总体来说，这段代码实现了一个简单的NX二次开发程序，可以打开用户指定的文本文件中的零件列表，逐个打开零件，并询问用户是否继续打开下一个零件。
>
