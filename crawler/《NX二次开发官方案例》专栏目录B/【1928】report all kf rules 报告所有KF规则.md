### 【1928】report all kf rules 报告所有KF规则

#### 代码

```cpp
    /*HEAD REPORT_ALL_KF_RULES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_kf.h>  
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
    static void report_kf_rules(char *name_chain)  
    {  
        int  
            ii,  
            n_rules;  
        char  
            child_name[UF_UI_MAX_STRING_LEN],  
            *class_name,  
            *rule_type,  
            **rules,  
            *value;  
        UF_KF_rule_p_t  
            rule;  
        UF_CALL(UF_KF_ask_rules(name_chain, &n_rules, &rules));  
        for (ii = 0; ii < n_rules; ii++)  
        {  
            UF_CALL(UF_KF_ask_rule_of_instance(name_chain, rules[ii], &rule));  
            UF_CALL(UF_KF_ask_rule_type(rule, &rule_type));  
            sprintf(child_name, "%s%s:", name_chain, rules[ii]);  
            ECHO("(%s) %s ", rule_type, child_name);  
            UF_CALL(UF_KF_evaluate_rule_to_string(child_name, &value));  
            if (!strcmp(rule_type, "Instance"))  
            {  
                UF_CALL(UF_KF_ask_instance_class(child_name, &class_name));  
                ECHO("(%s)\n", class_name);  
                report_kf_rules(child_name);  
            }  
            else  
            {  
                ECHO("%s\n", value);  
            }  
            UF_free(rule_type);  
            UF_free(value);  
        }  
        if (n_rules > 0) UF_free_string_array(n_rules, rules);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_KF_init_part(part));  
        report_kf_rules("root:");  
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

> 这段代码是一个NX Open C++应用程序，主要实现了以下功能：
>
> 1. 初始化和错误处理：使用UF_initialize初始化NX Open，并定义了UF_CALL宏来处理函数调用返回码，如果返回码不为0，则记录错误信息。
> 2. 规则查询：定义了report_kf_rules函数来递归查询并打印出NX部件的规则树，包括规则类型、实例名称、类名称和字符串表达式。
> 3. 主函数：主函数首先调用uc4624来解析命令行参数，然后循环调用uc4621获取每个部件名称，接着打开部件，打印加载状态，并调用do_it函数查询并打印规则。
> 4. 部件处理：do_it函数初始化部件，并调用report_kf_rules查询并打印整个规则树。
> 5. 命令行参数解析：程序支持直接指定部件路径或使用iman数据库中的部件标识符来打开部件。
> 6. 资源释放：在打开多个部件的情况下，最后调用UF_PART_close_all关闭所有部件。
> 7. 卸载处理：定义了ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY表示可以直接卸载动态库。
> 8. NX/UG模式支持：程序既可以在NX内部运行，也可以在外部以命令行方式运行。
> 9. 日志和窗口输出：通过ECHO函数将信息输出到日志窗口和NX日志文件。
>
> 总体来说，这段代码提供了一个查询并打印NX部件规则的功能，并考虑了错误处理、命令行参数解析和不同运行模式的支持。
>
