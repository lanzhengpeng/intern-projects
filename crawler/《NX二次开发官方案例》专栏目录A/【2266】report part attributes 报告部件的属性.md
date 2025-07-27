### 【2266】report part attributes 报告部件的属性

#### 代码

```cpp
    /*HEAD REPORT_PART_ATTRIBUTES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_attr.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog是在V18中新增的。 */  
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
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part();  
        int  
            ii,  
            num;  
        UF_ATTR_part_attr_t  
            *attrs;  
        UF_CALL(UF_ATTR_ask_part_attrs(part, &num, &attrs));  
        for (ii = 0; ii < num; ii++)  
        {  
            WRITE(attrs[ii].title);  
            WRITE(" = ");  
            WRITE(attrs[ii].string_value);  
            WRITE("\n");  
        }  
        if (num > 0) UF_free( attrs);  
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

> 这段代码是一个NX Open C++应用程序，主要功能是遍历并打印NX部件的属性。以下是代码的主要功能介绍：
>
> 1. 初始化和终止：使用UF_initialize()和UF_terminate()来初始化和终止NX Open API。
> 2. 错误报告函数：定义了一个名为report_error()的函数，用于在API调用失败时报告错误信息。
> 3. 获取部件属性：使用UF_ATTR_ask_part_attrs()函数获取指定部件的所有属性，并使用一个循环遍历所有属性并打印它们的名称和值。
> 4. 部件加载：在主函数中，通过解析命令行参数来加载指定的部件文件，并调用do_it()函数来打印部件属性。
> 5. 检查加载状态：使用check_load_status()函数来检查部件加载状态，并打印加载失败的信息。
> 6. 交互模式：代码支持交互模式和外部模式。在交互模式下，它会加载当前显示的部件并打印其属性。在外部模式下，它会加载命令行参数指定的部件文件并打印其属性。
> 7. 资源释放：使用UF_free()函数释放动态分配的内存。
> 8. 日志打印：使用UF_print_syslog()函数将错误信息打印到系统日志中。
> 9. 用户界面：使用UF_UI系列函数在用户界面窗口中打印信息。
>
> 总体来说，这段代码通过NX Open API遍历并打印了NX部件的属性信息，并提供了交互和外部两种运行模式。代码结构清晰，注释详细，便于理解和维护。
>
