### 【2170】report kf rule string 报告知识框架规则字符串

#### 代码

```cpp
    /*HEAD REPORT_KF_RULE_STRING CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_kf.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
    }  
    static logical prompt_for_text(char *prompt, char *text)  
    {  
    #ifdef _USRDLL  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    #else  
        printf(prompt);  
        printf(": (<Ctrl>C when done)\n");  
        if (gets(text)) return TRUE;;  
        return FALSE;  
    #endif  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            name_chain[133] = { "" },  
            *value;  
        UF_CALL(UF_KF_init_part(part));  
        while (prompt_for_text("Enter rule name chain", name_chain))  
        {  
            if (!UF_CALL(UF_KF_evaluate_rule_to_string(name_chain, &value)))  
            {  
                WRITE_S(value);  
                UF_free(value);  
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

> 这段代码是一个NX二次开发示例，主要用于报告NX中零件的规则字符串。代码的主要功能如下：
>
> 1. 定义了一些宏和函数，用于输出错误信息和字符串，方便调试。
> 2. do_it函数用于查询零件的当前显示零件，初始化零件的规则，然后循环读取用户输入的规则名称链，评估规则并输出规则字符串。
> 3. ufusr函数用于初始化NX，调用do_it函数，然后终止NX。
> 4. main函数是程序的入口，用于初始化NX，加载零件，调用do_it函数处理每个零件，然后关闭零件并终止NX。
> 5. 通过命令行参数可以指定要处理的零件文件，也可以通过UG/Manager启动时传递零件标签。
> 6. 使用了NX提供的UI函数，可以在UG的输出窗口中显示信息。
> 7. 通过错误处理函数report_error，可以输出函数调用失败时的错误码和错误信息。
> 8. 通过prompt_for_text函数，实现了交互式输入规则名称链。
>
> 综上所述，这段代码实现了基本的NX二次开发流程，包括初始化、处理零件、交互式输入和输出信息等。代码结构清晰，注释详细，便于学习和参考。
>
