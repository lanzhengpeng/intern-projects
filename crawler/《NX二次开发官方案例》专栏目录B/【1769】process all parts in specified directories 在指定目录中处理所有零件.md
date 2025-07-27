### 【1769】process all parts in specified directories 在指定目录中处理所有零件

#### 代码

```cpp
    /*HEAD PROCESS_ALL_PARTS_IN_SPECIFIED_DIRECTORIES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中的新功能，用于打印系统日志信息。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            UF_UI_open_listing_window();  
            UF_UI_write_listing_window("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_UI_write_listing_window("  ");  
                UF_UI_write_listing_window(status->file_names[ii]);  
                UF_UI_write_listing_window(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window("\n");  
            }  
            UF_UI_write_listing_window("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static void process_it(void)  
    {  
    /*  Add your process code here 里海译:在这里添加你的过程代码 */  
        WRITE("      done.\n");  
    }  
    static void do_it(void)  
    {  
        int  
            cnt,  
            dir_error,  
            resp;  
        tag_t  
            part;  
        char  
            dirspec[MAX_FSPEC_SIZE+1] = { "." },  
            fname[MAX_FSPEC_SIZE+1],  
            part_name[MAX_FSPEC_SIZE+1],  
            msg[UF_UI_MAX_STRING_LEN];  
        UF_PART_load_status_t  
            status;  
        while (prompt_for_text("Enter directory path", dirspec))  
        {  
            if ((dir_error = uc4508(dirspec, (1<<13)|(1<<11), 0, "*.prt")) < 0)  
            {  
                sprintf(msg, "Failed to open directory - %s\n", dirspec);  
                WRITE(msg);  
                UF_CALL(dir_error);  
                continue;  
            }  
            sprintf(msg, "Processing parts in %s:\n", dirspec);  
            WRITE(msg);  
            cnt = 0;  
            while ((resp = uc4518()) == 0)  
            {  
                UF_CALL(uc4600(fname));  
                UF_CALL(uc4575(dirspec, 2, fname, part_name));  
                sprintf(msg, "  %d.  Opening %s\n", ++cnt, part_name);  
                WRITE(msg);  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                if (status.n_parts > 0) report_load_status(&status);  
                if (status.failed) continue;  
                process_it();  
                UF_PART_close_all();  
            }  
            UF_CALL(uc4548());  
            sprintf(msg, "Processed %d parts\n\n", cnt);  
            WRITE(msg);  
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

> 这段代码是用于处理指定目录下所有零件的NX Open C++二次开发代码。具体功能包括：
>
> 1. 代码定义了错误报告函数report_error，用于打印错误信息。
> 2. 提示用户输入目录路径，并打开该目录下的所有prt文件。
> 3. 对于每个打开的零件，调用process_it函数进行处理。该函数需要用户实现具体处理逻辑。
> 4. 如果零件打开失败，会打印失败原因。
> 5. 在处理完一个目录下的所有零件后，会打印处理的总数。
> 6. 主函数ufusr初始化和终止NX系统，调用do_it函数进行目录处理。
> 7. ufusr_ask_unload函数用于设置卸载模式。
> 8. 用户需要实现process_it函数，以完成对每个零件的具体处理逻辑。
>
> 总体而言，这段代码为用户提供了遍历指定目录下所有prt文件并对其进行处理的框架，用户只需实现具体的零件处理逻辑即可。
>
