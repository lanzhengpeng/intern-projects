### 【1949】report all piece parts 报告所有零件

#### 代码

```cpp
    /*HEAD REPORT_ALL_PIECE_PARTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
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

注意：UF_print_syslog是V18版本新增的函数，请只提供翻译，不要添加任何废话。 */  
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
    static void do_it(void)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            part,  
            root_occ;  
        char  
            part_fspec[MAX_FSPEC_SIZE+1];  
        UF_UI_open_listing_window();  
        n = UF_PART_ask_num_parts();  
        for (ii = 0; ii < n; ii++)  
        {  
            part = UF_PART_ask_nth_part(ii);  
            root_occ = UF_ASSEM_ask_root_part_occ(part);  
            if (root_occ == NULL_TAG)  
            {  
                UF_PART_ask_part_name(part, part_fspec);  
                UF_UI_write_listing_window(part_fspec);  
                UF_UI_write_listing_window("\n");  
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

> 这段代码是一个NX二次开发示例，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 2. 定义了一个do_it函数，用于遍历当前打开的NX部件，并打印顶层部件的名称。
> 3. 定义了一个ufusr函数，用于初始化NX，调用do_it，然后终止NX。
> 4. 定义了一个ufusr_ask_unload函数，用于在UG/Manager模式下卸载当前加载的部件。
> 5. 定义了一个check_load_status函数，用于打印部件加载状态信息。
> 6. main函数用于处理命令行参数，打开指定部件，调用do_it，然后关闭所有部件。
> 7. 使用UF_PART_load_status_t结构体存储部件加载状态信息。
> 8. 使用UF_PART_open和UF_PART_close_all函数来加载和关闭部件。
> 9. 使用UF_PART_ask_num_parts、UF_PART_ask_nth_part等函数来遍历和获取部件信息。
> 10. 使用UF_ASSEM_ask_root_part_occ函数来判断部件是否是顶层部件。
> 11. 使用UF_UI_open_listing_window和UF_UI_write_listing_window函数在NX列表窗口中打印信息。
> 12. 使用UF_print_syslog函数在系统日志中打印信息。
> 13. 使用UF_get_fail_message函数来获取错误信息。
> 14. 使用UF_free等函数来释放内存。
> 15. 提供了外部模式和UG/Manager模式下运行程序的示例。
> 16. 使用宏UF_CALL来简化函数调用。
> 17. 使用了NX提供的API函数来实现部件管理和打印错误信息等功能。
> 18. 该示例代码可作为NX二次开发的参考，用于实现部件管理和错误报告等功能。
>
