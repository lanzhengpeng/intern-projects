### 【3130】open and close parts 打开和关闭部件

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
                ECHO("  %s - %s\n", status->file_names[ii], msg);  
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

> 这段代码是一个简单的NX Open二次开发程序，其主要功能是打开NX部件文件并输出相关信息。下面是代码的主要功能：
>
> 1. 头文件包含：包含必要的NX Open头文件，如uf.h, uf_ui.h等。
> 2. 错误处理宏：定义了UF_CALL和WRITE_L两个宏，用于简化错误处理和逻辑值输出。
> 3. 错误报告函数：定义了report_error函数，用于在出错时输出错误码和错误信息。
> 4. 逻辑值输出函数：定义了write_logical_to_listing_window函数，用于输出逻辑值。
> 5. 部件加载状态报告函数：定义了report_load_status函数，用于输出部件加载状态信息。
> 6. 主函数：初始化NX系统。检查NX是否处于管理器模式。循环调用uc4621获取部件名，并打开部件。输出部件名和加载状态。如果加载成功，则输出部件打开成功信息。循环结束后，输出处理过的部件数量。结束NX系统。
> 7. 初始化NX系统。
> 8. 检查NX是否处于管理器模式。
> 9. 循环调用uc4621获取部件名，并打开部件。
> 10. 输出部件名和加载状态。
> 11. 如果加载成功，则输出部件打开成功信息。
> 12. 循环结束后，输出处理过的部件数量。
> 13. 结束NX系统。
> 14. 注释说明：提供了在UG和NX管理器模式下运行程序的说明。
>
> 总体而言，这段代码实现了基本的NX部件文件打开、信息输出和错误处理功能，是一个典型的NX二次开发示例。
>
