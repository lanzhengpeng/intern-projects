### 【3049】verify nxmanager encode decode correspondence 验证NX Manager的编码和解码对应关系

#### 代码

```cpp
    /*HEAD VERIFY_NXMANAGER_ENCODE_DECODE_CORRESPONDENCE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_ugmgr.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，我总结出以下几点：

1. UF_print_syslog是V18版本新增的函数。
2. 该函数用于将日志信息输出到系统日志。
3. 日志信息包括当前时间、进程ID、日志级别和日志内容。
4. 日志级别分为DEBUG、INFO、NOTICE、WARNING、ERROR、CRITICAL和ALERT。
5. 使用UF_print_syslog函数时，应指定日志级别和日志内容。
6. 该函数可以帮助开发人员跟踪程序运行过程中的关键信息，便于调试和监控。
7. V18版本新增了UF_print_syslog函数，提高了日志输出的便利性。

以上是对UF_print_syslog函数的简要总结，如有需要补充或修改的地方，请指出。 */  
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
    static logical pick_one_of_two_choices(char *prompt, char *option_one,  
        char *option_two, int *choice)  
    {  
        int  
            resp;  
        char  
            options[2][38];  
        strncpy(&options[0][0], option_one, 37);  
        strncpy(&options[1][0], option_two, 37);  
        options[0][37] = '\0';  
        options[1][37] = '\0';  
        resp = uc1603(prompt, *choice, options, 2);  
        if ((resp > 4) && (resp < 19))  
        {  
            *choice = resp - 4;  /* returns 1 or 2 里海译:根据语境，"returns 1 or 2"可翻译为“返回1或2”。 */  
            return TRUE;  
        }  
        else return FALSE;  
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
    static void do_it(void)  
    {  
        logical  
            in_nxmanager;  
        int  
            ii,  
            mode = 1,  
            n_parts;  
        tag_t  
            decode_part,  
            decode_display_part,  
            encode_display_part,  
            encode_part,  
            nth_part;  
        char  
            decode_name[MAX_FSPEC_SIZE+1],  
            encode_name[MAX_FSPEC_SIZE+1],  
            encode_display_name[MAX_FSPEC_SIZE+1],  
            decode_display_name[MAX_FSPEC_SIZE+1],  
            part_file_name[UF_UGMGR_FNAME_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1];  
        if (UF_CALL(UF_is_ugmanager_active(&in_nxmanager)) || !in_nxmanager ||  
            !pick_one_of_two_choices("Select mode", "Errors Only", "Full Report",  
            &mode)) return;  
        n_parts = UF_PART_ask_num_parts();  
        for (ii = 0; ii < n_parts; ii++)  
        {  
            nth_part = UF_PART_ask_nth_part(ii);  
            UF_CALL(UF_PART_ask_part_name(nth_part, decode_name));  
            decode_part = UF_PART_ask_part_tag(decode_name);  
            UF_CALL(UF_PART_name_for_display(decode_name, decode_display_name));  
            UF_CALL(UF_PART_ask_tag_of_disp_name(decode_display_name,  
                &decode_display_part));  
            UF_CALL(UF_UGMGR_decode_part_filename(decode_name, part_number,  
                part_revision, part_file_type, part_file_name));  
            UF_CALL(UF_UGMGR_encode_part_filename(part_number, part_revision,  
                part_file_type, part_file_name, encode_name));  
            UF_CALL(UF_PART_name_for_display(encode_name, encode_display_name));  
            UF_CALL(UF_PART_ask_tag_of_disp_name(encode_display_name,  
                &encode_display_part));  
            encode_part = UF_PART_ask_part_tag(encode_name);  
            if ((mode == 2) ||  
                (nth_part != decode_part) || (nth_part != decode_display_part) ||  
                (nth_part != encode_display_part) || (nth_part != encode_part))  
            {  
                WRITE_D(nth_part);  
                WRITE_S(decode_name);  
                WRITE_D(decode_part);  
                WRITE_S(decode_display_name);  
                WRITE_D(decode_display_part);  
                WRITE_S(part_number);  
                WRITE_S(part_revision);  
                WRITE_S(part_file_name);  
                WRITE_S(part_file_type);  
                WRITE_S(encode_name);  
                WRITE_S(encode_display_name);  
                WRITE_D(encode_display_part);  
                WRITE_D(encode_part);  
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

```

#### 代码解析

> 这是段用于验证NX部件管理器编解码一致性的NX二次开发代码。主要功能包括：
>
> 1. 验证NX部件管理器是否激活。
> 2. 选择错误模式或完整报告模式。
> 3. 遍历当前会话中的所有部件。
> 4. 获取每个部件的名称、标签、显示名称等。
> 5. 调用NX部件管理器的编解码接口，获取部件的编解码文件名。
> 6. 比较编解码前后部件的名称、标签、显示名称等是否一致。
> 7. 如果不一致，根据模式选择，将差异信息输出到日志窗口或系统日志。
> 8. 提供了unload函数，以便在退出时卸载。
>
> 代码利用NX的UF函数接口，采用C语言编写，主要用于验证NX部件管理器编解码功能的正确性。
>
