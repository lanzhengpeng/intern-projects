### 【1944】report all open parts 报告所有打开的零件

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_ugmgr.h>  
    #include <stdarg.h>  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
    }  
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
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
            cli_name[UF_CFI_MAX_PATH_NAME_SIZE],  
            leaf_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        n = UF_PART_ask_num_parts();  
        for (ii = 0; ii < n; ii++)  
        {  
            part = UF_PART_ask_nth_part(ii);  
            WRITE_D(part);  
            UF_CALL(UF_PART_ask_part_name(part, part_fspec));  
            WRITE_S(part_fspec);  
            WRITE_D(UF_PART_is_loaded(part_fspec));  
            if (nxman)  
            {  
                UF_CALL(UF_UGMGR_convert_name_to_cli(part_fspec, cli_name));  
                WRITE_S(cli_name);  
                WRITE_D(UF_PART_is_loaded(cli_name));  
            }  
            else  
            {  
                UF_CALL(uc4574(part_fspec, 2, leaf_name));  
                WRITE_S(leaf_name);  
                WRITE_D(UF_PART_is_loaded(leaf_name));  
            }  
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
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                if (status.n_parts > 0) check_load_status(&status);  
                if (!status.failed)  
                {  
                    do_it();  
                    UF_CALL(UF_PART_close_all());  
                }  
            }  
            printf("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这是段NX Open C++的二次开发代码，主要功能是遍历NX部件库中的所有部件，并获取每个部件的名称、是否加载等信息，然后输出到日志窗口和系统日志中。
>
> 主要特点包括：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用出错时输出错误信息。
> 2. 定义了一个字符串输出函数write_string_to_listing_window，用于输出字符串变量到日志窗口。
> 3. 定义了一个数字输出宏WRITE_D，用于输出数字变量到日志窗口。
> 4. 主函数do_it中，遍历所有部件，获取部件名称、是否加载等信息，并输出。
> 5. 使用UF_UGMGR接口将部件名称转换为CLI名称。
> 6. 使用UF_PART接口判断部件是否加载。
> 7. 主函数main中，根据命令行参数打开部件，然后调用do_it函数。
> 8. 提供了unload时的处理函数ufusr_ask_unload。
> 9. 使用宏UF_CALL包装UF函数调用，用于报告错误。
> 10. 输出信息到日志窗口和系统日志。
>
> 总体来说，这段代码实现了基本的NX部件管理功能，并提供了完善的错误处理和日志输出，可以作为NX二次开发的参考示例。
>
