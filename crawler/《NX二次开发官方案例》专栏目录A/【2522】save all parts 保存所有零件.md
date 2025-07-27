### 【2522】save all parts 保存所有零件

#### 代码

```cpp
    /*HEAD SAVE_ALL_PARTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的内容，UF_print_syslog是在V18版本中新增的功能。文档要求只翻译内容，不添加其他无关信息。因此，根据文档内容，翻译如下：

UF_print_syslog是在V18版本中新增的功能。 */  
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
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        int  
            count = 0,  
            *error_list,  
            ii;  
        tag_t  
            *part_list;  
        char  
            fspec[MAX_FSPEC_SIZE+1],  
            msg[133];  
        UF_CALL(UF_PART_save_all(&count, &part_list, &error_list));  
        if (count > 0)  
        {  
            WRITE_D(count);  
            for (ii = 0; ii < count; ii++)  
            {  
                WRITE_D(ii);  
                WRITE_D(part_list[ii]);  
                UF_PART_ask_part_name(part_list[ii], fspec);  
                WRITE_S(fspec);  
                WRITE_D(error_list[ii]);  
                UF_get_fail_message(error_list[ii], msg);  
                WRITE_S(msg);  
            }  
            UF_free(part_list);  
            UF_free(error_list);  
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

> 这段代码是NX Open C++的一个示例，主要实现了保存当前打开的所有部件的功能。
>
> 代码的关键点包括：
>
> 1. 包含NX Open C++提供的头文件，定义了UF函数接口。
> 2. 定义了report_error函数，用于处理UF函数调用失败时的错误报告。
> 3. 定义了WRITE_D和WRITE_S宏，用于向列表窗口打印整型和字符串信息。
> 4. 定义了write_integer_to_listing_window和write_string_to_listing_window函数，用于向列表窗口打印整型和字符串信息。
> 5. 定义了do_it函数，用于保存所有部件并打印相关信息。
> 6. ufusr函数是NX Open C++程序的入口函数，用于初始化NX Open C++环境，调用do_it函数执行保存操作，然后终止NX Open C++环境。
> 7. ufusr_ask_unload函数用于指定程序卸载模式。
>
> 总的来说，这段代码实现了NX Open C++环境下的部件保存功能，并且能够打印出保存的部件名称和保存过程中发生的错误。
>
