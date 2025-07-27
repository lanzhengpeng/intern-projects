### 【1140】dummy out all expressions 将表达式中的所有值替换为0，使它们变成无意义的占位符

#### 代码

```cpp
    /*HEAD DUMMY_OUT_ALL_EXPRESSIONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 中的新功能，用于打印系统日志信息。在 V18 之前的版本中，这个功能并不存在。 */  
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
    static void strip_trailing_zeros(char *s)  
    {  
        int ii;  
        if (strchr(s, '.'))  
        {  
            for (ii = strlen(s) - 1; s[ii] == '0'; ii--) s[ii] = '\0';  
            if (s[ii] == '.') s[ii] = '\0';  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_exps;  
        tag_t  
            *exps,  
            part = UF_PART_ask_display_part();  
        double  
            exp_val;  
        char  
            *lhs,  
            new_exp[UF_MAX_EXP_LENGTH+1],  
            *old_exp,  
            *rhs;  
        UF_CALL(UF_UI_open_listing_window());  
        UF_CALL(UF_MODL_ask_exps_of_part(part, &n_exps, &exps));  
        for (ii = 0; ii < n_exps; ii++)  
        {  
            UF_CALL(UF_MODL_ask_exp_tag_value(exps[ii], &exp_val));  
            UF_CALL(UF_MODL_ask_exp_tag_string(exps[ii], &old_exp));  
            UF_CALL(UF_MODL_dissect_exp_string(old_exp, &lhs, &rhs, &exps[ii]));  
            sprintf(new_exp, "%s=%f", lhs, exp_val);  
            UF_free(lhs);  
            UF_free(rhs);  
            strip_trailing_zeros(new_exp);  
            if (strstr(old_exp, " //")) strcpy(strstr(old_exp, " //"), "");  
            if (strcmp(new_exp, old_exp))  
            {  
                UF_CALL(UF_UI_write_listing_window(old_exp));  
                UF_CALL(UF_UI_write_listing_window("\n"));  
                UF_CALL(UF_UI_write_listing_window(new_exp));  
                UF_CALL(UF_UI_write_listing_window("\n"));  
                UF_CALL(UF_MODL_edit_exp(new_exp));  
            }  
            UF_free(old_exp);  
        }  
        UF_free(exps);  
        UF_CALL(UF_MODL_update());  
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
    static void build_copy_partname(char *copy_fspec)  
    {  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE];  
        tag_t  
            part = UF_PART_ask_display_part();  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
        UF_CALL(uc4578(part_name, 2, part_name));  
        strcat(part_name, "-new");  
        UF_CALL(uc4575(dspec, 2, part_name, copy_fspec));  
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
            new_fspec[MAX_FSPEC_SIZE+1],  
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
                    build_copy_partname(new_fspec);  
                    if (!UF_CALL(UF_PART_save_as(new_fspec)))  
                        printf("\tsaved as %s\n", new_fspec);  
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

> 这是段NX二次开发代码，主要功能是遍历零件中的表达式，将每个表达式的值输出到列表窗口。具体来说，代码实现了以下功能：
>
> 1. 初始化和错误报告：在UF_initialize中初始化NX，并定义了report_error函数用于输出错误信息。
> 2. 遍历零件中的表达式：使用UF_MODL_ask_exps_of_part获取零件中的表达式，然后遍历每个表达式，使用UF_MODL_ask_exp_tag_value获取表达式的值，UF_MODL_ask_exp_tag_string获取表达式的字符串，UF_MODL_dissect_exp_string分解表达式字符串为左值和右值。
> 3. 输出表达式的值：将表达式的值格式化为字符串，去除尾部的0，并与原始表达式字符串进行比较。如果不同，则将原始表达式和新表达式都输出到列表窗口。
> 4. 保存和关闭零件：如果零件加载成功，则执行上述遍历表达式并输出值的功能，然后保存零件为新的文件，并关闭所有打开的零件。
> 5. 主函数：主函数解析命令行参数，加载每个零件，并执行上述功能。另外，提供了在UG外部模式和内部模式下的运行方式。
>
> 总的来说，这段代码实现了在NX中遍历零件中的表达式，并将每个表达式的值输出到列表窗口的功能。
>
