### 【0345】ask printers of all groups 查询所有打印机组中的打印机

#### 代码

```cpp
    /*HEAD ASK_PRINTERS_OF_ALL_GROUPS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_plot.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文中的内容，UF_print_syslog是在V18版本中新增的，请只回答翻译，不要添加其他无关内容。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void ensure_directory_exists(char *dir)  
    {  
        char  
            folder[UF_CFI_MAX_FILE_NAME_SIZE],  
            parent[UF_CFI_MAX_PATH_NAME_SIZE];  
        if (uc4560(dir, 100))  
        {  
            UF_CALL(uc4576(dir, 100, parent, folder));  
            ensure_directory_exists(parent);  
            UF_CALL(uc4563(dir, 100));  
        }  
    }  
    static void ensure_parent_directory_exists(char *dir)  
    {  
        char  
            folder[UF_CFI_MAX_FILE_NAME_SIZE],  
            parent[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_CALL(uc4576(dir, 100, parent, folder));  
        ensure_directory_exists(parent);  
    }  
    static void do_it(void)  
    {  
        int  
            ii, jj, kk,  
            n_groups,  
            n_profiles,  
            n_printers;  
        char  
            **profiles,  
            **printers;  
        UF_PLOT_printer_group_t  
            *groups;  
        UF_CALL(UF_PLOT_ask_printer_groups( &n_groups, &groups ));  
        WRITE_D(n_groups);  
        for (ii = 0; ii < n_groups; ii++)  
        {  
            WRITE_D(ii);  
            WRITE_S(groups[ii].label);  
            WRITE_S(groups[ii].group_dir);  
            WRITE_S(groups[ii].jobs_dir);  
            WRITE_S(groups[ii].home_dir);  
        /*  Start work around to PR 5373882 译:开始围绕PR 5373882进行工作。 */  
            ensure_parent_directory_exists(groups[ii].jobs_dir);  
            ensure_parent_directory_exists(groups[ii].home_dir);  
        /*  End work around to PR 5373882 译:结束对PR 5373882的工作围绕，只回答译文，不要废话 */  
            UF_CALL(UF_PLOT_set_printer_group(groups[ii].group_dir,  
                groups[ii].jobs_dir, groups[ii].home_dir));  
            UF_CALL(UF_PLOT_ask_printer_names(&n_printers, &printers));  
            WRITE_D(n_printers);  
            for (jj = 0; jj < n_printers; jj++)  
            {  
                WRITE_D(jj);  
                WRITE_S(printers[jj]);  
                UF_CALL(UF_PLOT_ask_profile_names(printers[jj], &n_profiles,  
                    &profiles));  
                WRITE_D(n_profiles);  
                for (kk = 0; kk < n_profiles; kk++)  
                {  
                    WRITE_D(kk);  
                    WRITE_S(profiles[kk]);  
                }  
                if (n_profiles > 0) UF_free_string_array(n_profiles, profiles);  
            }  
            if (n_printers > 0) UF_free_string_array(n_printers, printers);  
        }  
        if (n_groups > 0) UF_free(groups);  
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
    /*  To run in external native mode UG:  
            program.exe  
        To run in external UG/Manager or IMAN mode:  
            program.exe -pim=yes  
    */  
    int main( int argc, char *argv[] )  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是用于NX的二次开发，主要功能是查询NX中的所有打印机组，并打印出每个组的信息，包括组名、目录路径等。此外，还会打印出每个打印机组下的所有打印机名称和配置文件名。以下是代码的主要功能和流程：
>
> 1. 定义了一个错误报告函数report_error，用于打印出函数调用失败时的错误信息。
> 2. 定义了整数和字符串的打印函数，用于将信息输出到列表窗口。
> 3. 定义了确保目录存在的函数ensure_directory_exists，用于创建NX中的目录。
> 4. do_it函数实现了主要功能，包括：调用UF_PLOT_ask_printer_groups查询所有打印机组信息，并打印出组名和目录路径。调用UF_PLOT_set_printer_group设置当前打印机组。调用UF_PLOT_ask_printer_names查询当前组下的所有打印机名称，并打印。遍历每个打印机，调用UF_PLOT_ask_profile_names查询配置文件名，并打印。
> 5. 调用UF_PLOT_ask_printer_groups查询所有打印机组信息，并打印出组名和目录路径。
> 6. 调用UF_PLOT_set_printer_group设置当前打印机组。
> 7. 调用UF_PLOT_ask_printer_names查询当前组下的所有打印机名称，并打印。
> 8. 遍历每个打印机，调用UF_PLOT_ask_profile_names查询配置文件名，并打印。
> 9. ufusr函数是NX调用的主函数，用于初始化、调用do_it函数，然后终止。
> 10. main函数用于在命令行模式下运行此程序，同样实现了初始化、调用do_it、终止的功能。
> 11. ufusr_ask_unload函数用于立即卸载此程序。
>
> 综上，这段代码主要用于查询和打印NX中的打印机组、打印机和配置文件信息，实现了NX二次开发中的查询功能。
>
