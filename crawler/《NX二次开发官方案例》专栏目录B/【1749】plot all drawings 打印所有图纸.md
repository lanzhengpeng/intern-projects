### 【1749】plot all drawings 打印所有图纸

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_plot.h>  
    #include <uf_draw.h>  
    #include <uf_obj.h>  
    #include <uf_cfi.h>  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
    }  
    static void plot_dwgs(char *printer, char *profile)  
    {  
        int  
            ii,  
            n_dwgs;  
        tag_t  
            *dwgs;  
        char  
            drawing_name[MAX_ENTITY_NAME_SIZE+1],  
            *job_name;  
        UF_CALL(UF_DRAW_ask_drawings(&n_dwgs, &dwgs));  
        for (ii = 0; ii < n_dwgs; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_name(dwgs[ii], drawing_name));  
            WRITE_S(drawing_name);  
            UF_CALL(UF_PLOT_ask_default_job_name(dwgs[ii], &job_name));  
            WRITE_S(job_name);  
            UF_CALL(UF_PLOT_print(dwgs[ii], NULL, job_name, NULL, printer, profile, 1));  
            UF_free(job_name);  
        }  
        if (n_dwgs > 0) UF_free(dwgs);  
    }  
    static void do_it(void)  
    {  
        char  
            *printer,  
            *profile;  
    /*  This does not work on HP-UX when using Printer Groups - see PR 5722700 里海译:在HP-UX系统上使用打印机组时，此功能不起作用。请参阅PR 5722700。 */  
        UF_CALL(UF_PLOT_ask_default_printer_and_profile( &printer, &profile));  
        WRITE_S(printer);  
        WRITE_S(profile);  
        plot_dwgs(printer, profile);  
        UF_free(printer);  
        UF_free(profile);  
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
            program.exe "group name" "printer name" "profile name" /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external UG/Manager or IMAN mode:  
            program.exe -pim=yes "group name" "printer name" "profile name" @DB/partno1/rev @DB/partno2/rev [...]  
    */  
    int main( int argc, char *argv[] )  
    {  
        int  
            ii,  
            n_groups;  
        tag_t  
            part;  
        char  
            group[MAX_ENTITY_NAME_SIZE+1],  
            part_name[MAX_FSPEC_SIZE+1],  
            printer[MAX_ENTITY_NAME_SIZE+1],  
            profile[MAX_ENTITY_NAME_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        UF_PLOT_printer_group_t  
            *groups;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            if ((uc4621(group) == 1 ) &&  
                (uc4621(printer) == 1 ) &&  
                (uc4621(profile) == 1 ))  
            {  
                WRITE_S(group);  
                WRITE_S(printer);  
                WRITE_S(profile);  
            }  
            if (strcmp(group, ""))  
            {  
                UF_CALL(UF_PLOT_ask_printer_groups(&n_groups, &groups));  
                for (ii = 0; ii < n_groups; ii++)  
                {  
                    if (!strcmp(groups[ii].label, group))  
                    {  
                        UF_CALL(UF_PLOT_set_printer_group(groups[ii].group_dir,  
                            groups[ii].jobs_dir, groups[ii].home_dir));  
                        break;  
                    }  
                }  
                if (n_groups > 0) UF_free(groups);  
            }  
            while (uc4621(part_name) == 1)  
            {  
                WRITE_S(part_name);  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                if (status.n_parts > 0) check_load_status(&status);  
                if (!status.failed)  
                {  
                    plot_dwgs(printer, profile);  
                    UF_CALL(UF_PART_close_all());  
                }  
            }  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++的二次开发示例，其主要功能是打印NX图纸。
>
> 代码的主要逻辑如下：
>
> 1. 定义了一个错误报告函数report_error，用于输出函数调用失败时的错误信息。
> 2. 定义了一个写入列表窗口的函数write_string_to_listing_window，用于输出信息到NX的列表窗口。
> 3. 定义了一个打印图纸的函数plot_dwgs，它会获取当前打开的所有图纸，逐个打印。
> 4. 定义了一个主函数do_it，它会获取默认的打印机名和打印配置文件名，然后调用plot_dwgs打印所有图纸。
> 5. 定义了ufusr函数，这是NX二次开发的入口函数，会初始化NX环境，调用do_it函数，然后清理NX环境。
> 6. 定义了main函数，这是程序的入口点。它会解析命令行参数，打开指定的NX部件文件，然后调用plot_dwgs打印图纸。
> 7. 代码使用了UF_PART模块打开部件文件，UF_PLOT模块打印图纸，UF_DRAW模块获取图纸，UF_OBJ模块获取图纸名称。
> 8. 代码中包含了一些错误处理的宏，如UF_CALL，用于简化错误处理。
> 9. 代码支持直接运行打印指定部件，也支持在NX内部调用打印。
>
> 总的来说，这段代码实现了一个简单的NX图纸打印功能，可以作为NX二次开发的参考示例。
>
