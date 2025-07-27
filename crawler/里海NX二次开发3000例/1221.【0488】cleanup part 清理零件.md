### 【0488】cleanup part 清理零件

#### 代码

```cpp
    /*HEAD CLEANUP_PART CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：

注意：UF_print_syslog在V18版本中是新增的。 */  
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
        UF_CALL(UF_PART_cleanup(UF_PART_cleanup_all));  
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
                    UF_CALL(UF_PART_save());  
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

> 这段NX二次开发代码的主要功能是清理NX部件文件中的数据，以便减少文件大小。具体来说，代码的主要部分包括：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 2. 清理函数：定义了一个do_it函数，其中调用了UF_PART_cleanup函数来清理部件文件中的数据。该函数接受一个参数，用于指定清理的类型，这里使用UF_PART_cleanup_all表示清理所有类型的数据。
> 3. UFusr函数：实现了ufusr函数，其中初始化NX，调用do_it函数清理数据，然后终止NX。这是NX二次开发的标准入口函数。
> 4. 主函数：在主函数中，首先初始化NX，然后读取命令行传入的部件文件名，并打开每个文件。如果文件打开成功，就调用do_it函数清理数据，然后保存并关闭文件。在所有文件处理完毕后，输出处理过的部件数量，然后终止NX。
> 5. 清理状态检查函数：定义了一个check_load_status函数，用于打印文件打开失败的状态信息。
> 6. 注释：代码中提供了详细的中文注释，解释了每个函数的作用以及整体流程。
>
> 这段代码的作用是清理NX部件文件中的数据，减小文件体积，提高系统性能。通过命令行传入文件名，可以批量处理多个文件。
>
