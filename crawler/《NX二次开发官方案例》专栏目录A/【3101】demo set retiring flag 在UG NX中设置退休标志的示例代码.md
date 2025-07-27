### 【3101】demo set retiring flag 在UG NX中设置退休标志的示例代码

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_attr.h>  
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
    #define WRITE_L(X) if (X) ECHO("%s = TRUE\n", #X); \  
        else ECHO("%s = FALSE\n", #X);  
    static void do_it(void)  
    {  
        UF_ATTR_value_t value;  
        UF_CALL( UF_set_retiring_flag( 2 ) );  
        WRITE_L( UF_OBJ_is_type_displayable( UF_shed_material_type ));  
        value.value.string = "Is UF_ATTR_assign retiring?";  
        UF_CALL(UF_ATTR_assign(UF_PART_ask_display_part(), "GTAC", value));  
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
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external NX Manager mode:  
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
                ECHO( "%d.  %s\n", ++cnt, part_name);  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                if (status.n_parts > 0) report_load_status(&status);  
                if (!status.failed)  
                {  
                    do_it();  
                    UF_CALL(UF_PART_close_all());  
                }  
            }  
            ECHO("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个NX二次开发示例，主要用于演示如何在NX中加载多个零件并执行一些操作。以下是代码的主要功能和流程：
>
> 1. 定义了ECHO宏，用于输出信息到列表窗口和系统日志。
> 2. 定义了UF_CALL宏，用于调用UF函数并报告错误。
> 3. 定义了WRITE_L宏，用于输出变量值到列表窗口。
> 4. 定义了do_it函数，用于设置零件的显示类型，并给零件赋值一个字符串属性。
> 5. 定义了ufusr函数，用于初始化NX环境，调用do_it函数，然后终止NX环境。
> 6. 定义了report_load_status函数，用于输出零件加载的状态信息。
> 7. 在main函数中，首先初始化NX环境，然后通过命令行参数加载零件。对于每个零件，调用do_it函数执行操作，并输出零件加载状态信息。最后统计加载的零件数量，并终止NX环境。
> 8. main函数可以通过外部命令行参数指定零件路径加载，也可以通过NX Manager数据库路径加载零件。
>
> 通过这个示例，可以了解如何在NX中加载零件、调用UF函数、输出信息、处理错误等，为NX二次开发提供参考。
>
