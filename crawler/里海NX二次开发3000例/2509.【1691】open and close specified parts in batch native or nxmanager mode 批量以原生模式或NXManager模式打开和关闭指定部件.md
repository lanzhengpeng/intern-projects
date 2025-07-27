### 【1691】open and close specified parts in batch native or nxmanager mode 批量以原生模式或NXManager模式打开和关闭指定部件

#### 代码

```cpp
    /*HEAD OPEN_AND_CLOSE_PARTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
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
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        if (flag)  
            ECHO("%s = TRUE\n", title);  
        else  
            ECHO("%s = FALSE\n", title);  
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
                ECHO("  %s - %s", status->file_names[ii], msg);  
            }  
            ECHO("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    /*  To run in external native mode UG:  
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external UG/Manager or IMAN mode:  
            program.exe -pim=yes @DB/partno1/rev @DB/partno2/rev [...]  
    */  
    int main( int argc, char *argv[] )  
    {  
        logical  
            nxmanager_mode;  
        int  
            cnt = 0;  
        tag_t  
            part;  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            UF_CALL(UF_is_ugmanager_active(&nxmanager_mode));  
            WRITE_L(nxmanager_mode);  
            while (uc4621(part_name) == 1)  
            {  
                ECHO("%d.  %s\n", ++cnt, part_name);  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                if (status.n_parts > 0) report_load_status(&status);  
                if (!status.failed)  
                {  
                    if (!UF_PART_ask_part_name(part, part_name))  
                    {  
                        ECHO("\t%s opened OK.\n", part_name);  
                        UF_PART_close_all();  
                    }  
                }  
            }  
            ECHO("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是用于NX Open的C++程序，其主要功能是打开指定的NX部件文件，并输出部件名称。
>
> 主要特点包括：
>
> 1. 使用NX Open的UF模块进行二次开发，包括UF模块初始化和部件操作等。
> 2. 通过命令行参数获取要打开的部件文件路径。
> 3. 使用循环读取所有要打开的部件文件，并调用UF_PART_open打开，输出打开的部件名称。
> 4. 使用宏定义UF_CALL检查UF函数调用是否成功，并输出错误信息。
> 5. 使用自定义的ECHO函数输出信息到控制台和NX日志窗口。
> 6. 在打开部件前判断是否在NX Manager模式下，如果是，则不打开部件。
> 7. 打开部件后，判断打开是否成功，并输出成功打开的部件名称。
> 8. 最后输出成功处理的部件数量，并释放资源。
>
> 总体来说，这段代码通过NX Open API实现了读取命令行参数并打开指定的NX部件文件，输出相关信息，并对错误进行了处理。
>
