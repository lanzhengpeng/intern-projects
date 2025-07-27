### 【1217】export gif in external mode 在UG外部模式下导出GIF

#### 代码

```cpp
    /*  
    The "gif.htt" file referenced by this application is a simple text file containing only this line:  
    <@UGSHADE FILE="<@UGVAR FORMAT="$HSPEC">.gif">  
    */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_web.h>  
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
    static void do_it(void)  
    {  
        char  
            fspec[MAX_FSPEC_SIZE+1];  
        tag_t  
            part = UF_PART_ask_display_part();  
        UF_PART_ask_part_name(part, fspec);  
        UF_CALL(uc4578(fspec, 2, fspec));  
        UF_CALL(UF_WEB_author_html(fspec, "<yourpath>\\gif.htt", FALSE));  
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
    static void check_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        for (ii=0; ii<status->n_parts; ii++)  
        {  
            UF_get_fail_message(status->statuses[ii], msg);  
            printf("    %s - %s\n", status->file_names[ii], msg);  
        }  
        UF_free(status->statuses);  
        UF_free_string_array(status->n_parts, status->file_names);  
    }  
    /*  To run in external native mode UG:  
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external UG/Manager or IMAN mode:  
            program.exe -pim=yes @DB/partno1/rev @DB/partno2/rev [...]  
    */  
    int main( int argc, char *argv[] )  
    {  
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
            while (uc4621(part_name) == 1)  
            {  
                printf("%d.  %s\n", ++cnt, part_name);  
                if (UF_CALL(UF_PART_open(part_name, &part, &status))  
                    || status.n_parts > 0) check_load_status(&status);  
                else  
                {  
                    do_it();  
                    UF_PART_close_all();  
                }  
            }  
            printf("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++应用程序，其主要功能是处理NX部件文件，并生成相应的HTML文件。以下是代码的详细介绍：
>
> 1. 包含了必要的NX Open C++头文件，以调用NX提供的API函数。
> 2. 定义了ECHO函数，用于在列表窗口和系统日志中输出信息。
> 3. 定义了UF_CALL宏，用于调用NX API函数并报告错误。
> 4. 定义了report_error函数，用于报告API函数调用错误。
> 5. 定义了do_it函数，用于处理一个NX部件文件：获取部件名称，调用uc4578函数，然后调用UF_WEB_author_html函数生成HTML文件。
> 6. 定义了ufusr函数，这是NX二次开发应用程序的入口函数。在此函数中，首先调用UF_initialize初始化NX环境，然后调用do_it函数处理部件，最后调用UF_terminate终止NX环境。
> 7. 定义了ufusr_ask_unload函数，返回卸载类型。
> 8. 定义了check_load_status函数，用于检查部件加载状态并打印错误信息。
> 9. main函数是程序的入口点。它首先初始化NX环境，然后循环调用uc4621获取传入的部件文件名，打开部件并调用do_it函数处理，最后打印已处理的部件数量并终止NX环境。
> 10. 注释提供了程序的运行说明，包括外部模式和NX/Manager模式下运行命令。
>
> 综上所述，这段代码实现了NX部件文件的处理，并生成了相应的HTML文件。它通过NX Open C++ API调用实现了NX与外部程序的交互。
>
