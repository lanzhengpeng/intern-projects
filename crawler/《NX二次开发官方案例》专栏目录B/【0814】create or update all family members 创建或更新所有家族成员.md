### 【0814】create or update all family members 创建或更新所有家族成员

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_fam.h>  
    #include <uf_assem.h>  
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
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    #define WRITENZ(X) if (X != 0) ECHO("%s = %d\n", #X, X);  
    #define WRITE_L(X) if (X) ECHO("%s = TRUE\n", #X); \  
        else ECHO("%s = FALSE\n", #X);  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
    }  
    #define RPN(X) (report_part_name(#X, X))  
    static void report_part_name(char *which, tag_t part)  
    {  
        char  
            fspec[MAX_FSPEC_SIZE+1];  
        UF_PART_ask_part_name(part, fspec);  
        ECHO("%s = %s\n", which, fspec);  
    }  
    static void do_it(void)  
    {  
        logical  
            is_template,  
            saved;  
        int  
            *errs,  
            ii, jj,  
            n_errs,  
            n_fams;  
        tag_t  
            *err_parts,  
            *fams,  
            instance,  
            mem_part,  
            prev,  
            part = UF_PART_ask_display_part();  
        char  
            *info,  
            msg[133],  
            *part_name,  
            part_spec[MAX_FSPEC_SIZE+1];  
        UF_FAM_family_data_t  
            family_data;  
        UF_CALL(UF_UI_open_listing_window());  
        UF_CALL(UF_PART_is_family_template(part, &is_template));  
        if (is_template)  
        {  
            UF_CALL(UF_PART_ask_families(part, &n_fams, &fams));  
            if (n_fams > 0)  
            {  
                UF_CALL(UF_FAM_ask_family_data( fams[0], &family_data));  
                for (ii = 0; ii < family_data.member_count; ii++)  
                {  
                    UF_CALL(UF_PART_find_family_instance(fams[0], ii, FALSE, FALSE,  
                        &part_name));  
                    if (!part_name)  
                    {  
                        UF_CALL(UF_PART_create_family_instance(fams[0], ii,  
                            &mem_part, &instance));  
                        UF_PART_ask_part_name(mem_part, part_spec);  
                        ECHO("creating member - %s\n", part_spec);  
                        UF_CALL(UF_ASSEM_set_work_part_quietly(mem_part, &prev));  
                        UF_CALL(UF_PART_save());  
                        UF_CALL(UF_PART_close(mem_part, 0, 1));  
                        UF_CALL(UF_ASSEM_set_work_part_quietly(prev, &mem_part));  
                    }  
                    else  
                    {  
                        ECHO("member exists - updating %s\n", part_name);  
                        UF_free(part_name);  
                        UF_CALL(UF_PART_update_family_instance(fams[0], ii, TRUE,  
                            &mem_part, &saved, &n_errs, &err_parts, &errs, &info));  
                        RPN(mem_part);  
                        WRITE_L(saved);  
                        if (saved) UF_CALL(UF_PART_close(mem_part, 0, 1));  
                        WRITE_D(n_errs);  
                        if (n_errs > 0)  
                        {  
                            for (jj = 0; jj < n_errs; jj++)  
                            {  
                                RPN(err_parts[jj]);  
                                UF_get_fail_message(errs[jj], msg);  
                                ECHO("%s\n", msg);  
                            }  
                            UF_free(err_parts);  
                            UF_free(errs);  
                        }  
                        if (info)  
                        {  
                            WRITE_S(info);  
                            UF_free(info);  
                        }  
                    }  
                }  
            }  
            else  
                ECHO("  No families found!\n");  
        }  
        else  
            ECHO("  Not a part family template!\n");  
    }  
    /* 里海 */  
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
    /*  To run in external native mode UG:  
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external NX/Manager mode:  
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
                report_load_status(&status);  
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

> 这段代码是一个NX二次开发示例，主要功能是检查并更新NX部件族模板的实例成员。下面是对代码的简要介绍：
>
> 1. 头文件包含：代码包含了一些NX二次开发常用的头文件，如uf.h、uf_ui.h、uf_cfi.h等。
> 2. 宏定义：定义了几个宏，用于错误报告和输出调试信息。
> 3. 错误报告函数：定义了report_error函数，用于在出错时输出错误码和错误信息。
> 4. 部件族处理函数：do_it函数是主函数，用于处理部件族模板。它首先判断当前打开的部件是否为部件族模板，如果是，则获取所有成员，并依次检查每个成员实例是否存在。如果不存在，则创建新的实例；如果存在，则更新实例。对于更新的实例，会输出更新结果和错误信息。
> 5. UFusr函数：ufusr是NX二次开发的入口函数，用于初始化和执行主体函数。
> 6. 主函数：主函数用于处理命令行传入的部件文件路径，依次打开每个部件，并调用do_it函数处理。
> 7. 部件加载状态报告函数：report_load_status用于输出部件加载的状态信息。
>
> 总的来说，这段代码实现了检查并更新NX部件族模板实例成员的功能，包含了二次开发常用的错误处理、信息输出、文件处理等内容，是一个典型的NX二次开发示例。
>
