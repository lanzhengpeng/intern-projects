### 【0576】create and edit kf rule 创建和编辑知识框架规则

#### 代码

```cpp
    /*HEAD CREATE_AND_EDIT_KF_RULE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_kf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，翻译为：注意：UF_print_syslog 是在 V18 版本中新增的函数，请只回答与译文相关的内容，不要添加无关的废话。 */  
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
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            *value;  
        UF_CALL(UF_KF_init_part(part));  
        UF_CALL(UF_KF_create_rule("root:", "aaNumber", "Modifiable Number",  
            "1", ""));  
        UF_CALL(UF_KF_create_rule("root:", "aaNumber", "Modifiable Number",  
            "2", ""));  
        if (!UF_CALL(UF_KF_evaluate_rule_to_string("root:aaNumber:", &value)))  
        {  
            WRITE_S(value);  
            UF_free(value);  
        }  
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

> 这段NX二次开发代码的主要功能是创建和编辑知识框架(Knowledge Framework)规则。
>
> 主要步骤包括：
>
> 1. 包含NX二次开发所需的头文件。
> 2. 定义了错误报告函数report_error，用于打印错误信息。
> 3. 定义了write_string_to_listing_window函数，用于将字符串写入列表窗口。
> 4. do_it函数中初始化知识框架，并创建两个规则。然后评估规则，并将结果字符串写入列表窗口。
> 5. ufusr函数是NX二次开发的入口函数，调用do_it函数。
> 6. ufusr_ask_unload函数用于卸载二次开发应用程序。
>
> 总体来说，这段代码通过NX提供的知识框架API，实现了规则的创建和编辑，以及规则结果的输出，是典型的NX二次开发知识框架应用。
>
