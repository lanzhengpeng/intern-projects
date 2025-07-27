### 【0771】create multiline note 创建多行注释

#### 代码

```cpp
    /*HEAD CREATE_MULTILINE_NOTE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：注意，UF_print_syslog是在V18中新增的。 */  
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
        tag_t  
            note;  
        char  
            *text[5],  
            strings[][20] = {"Note line 1", "Note line 2", "Note line 3",   
                             "Note line 4", "Note line 5"};  
        double  
            origin[3] = { 1, 1, 0 };  
        text[0] = &strings[0][0];  
        text[1] = &strings[1][0];          
        text[2] = &strings[2][0];          
        text[3] = &strings[3][0];           
        text[4] = &strings[4][0];  
        /* Multiple Line Note 译:多行笔记 */  
        UF_CALL(UF_DRF_create_note(5, text, origin, 0, &note));  
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

```

#### 代码解析

> 这段代码是一个NX Open C++程序，用于创建一个多行注释。具体来说，主要功能包括：
>
> 1. 包含必要的NX Open头文件，如uf.h、uf_ui.h和uf_drf.h。
> 2. 定义了一个错误报告函数report_error，用于打印出函数调用失败的错误信息。
> 3. 定义了一个do_it函数，用于创建一个包含5行文本的多行注释。文本内容定义在一个字符串数组中，位置设置为原点(1,1,0)。
> 4. ufusr函数是程序的入口点，用于初始化和结束NX Open，并调用do_it函数。
> 5. ufusr_ask_unload函数用于设置立即卸载。
>
> 总体来说，这段代码通过调用UF_DRF_create_note函数，在NX中创建一个包含多行文本的注释。注释的文本内容和位置都是自定义的，展示了NX Open C++在注释创建方面的基本用法。
>
