### 【3132】open close same part many times 多次打开和关闭同一个部件

#### 代码

```cpp
    #include <stdio.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
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
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            ECHO("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                ECHO("  ");  
                ECHO(status->file_names[ii]);  
                ECHO(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                ECHO(msg);  
                ECHO("\n");  
            }  
            ECHO("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    /*  To run in external native mode NX:  
            program.exe /path/to/part1.prt n_times  
        To run in external NX Manager mode:  
            program.exe -pim=yes @DB/partno1/rev n_times  
    */  
    int main( int argc, char *argv[] )  
    {  
            int  
                ii,  
                n;  
        int  
            cnt = 0;  
        tag_t  
            part;  
        char  
            times[MAX_FSPEC_SIZE+1],  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            if ((uc4621(part_name) == 1) && (uc4621(times) == 1))  
            {  
                sscanf(times, "%d", &n);  
                ECHO("Loading %s %d times...\n", part_name, n);  
                for (ii = 0; ii < n; ii++)  
                {  
                    if ((ii % 100) == 0)  
                        ECHO("%d...\n", ii);  
                    UF_CALL(UF_PART_open(part_name, &part, &status));  
                    if ((status.n_parts > 0) && (ii = 0))  
                        report_load_status(&status);  
                    else  
                    {  
                        UF_free(status.statuses);  
                        UF_free_string_array(status.n_parts, status.file_names);  
                    }  
                    if (status.failed) break;  
                    UF_CALL(UF_PART_close_all());  
                }  
            }  
            else  
                ECHO("Usage: %s \"<part to open & close>\" <loop count>\n", argv[0]);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个NX的二次开发程序，主要实现了以下功能：
>
> 1. 初始化和终止NX环境：通过UF_initialize()和UF_terminate()函数初始化和终止NX环境。
> 2. 打印日志信息：通过ECHO()函数，可以将信息打印到日志窗口和控制台。
> 3. 错误报告：通过report_error()函数，可以将函数调用失败的信息打印到日志窗口。
> 4. 打印加载状态：通过report_load_status()函数，可以打印出模型加载的状态信息。
> 5. 主函数：读取命令行参数，打开指定的零件文件，循环执行打开和关闭操作指定的次数。在打开和关闭过程中，会打印加载状态，如果打开失败会退出循环。
> 6. 使用宏定义UF_CALL()来封装函数调用，方便错误处理。
> 7. 支持两种模式，直接指定零件路径和次数，或者通过NX Manager的部件号和版本打开。
> 8. 释放资源：在打开模型失败时，释放分配的内存。
>
> 总的来说，这个程序通过命令行参数控制，实现了对指定零件文件的循环打开和关闭，并打印出加载状态和错误信息。
>
