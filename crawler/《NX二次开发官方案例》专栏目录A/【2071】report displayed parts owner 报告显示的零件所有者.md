### 【2071】report displayed parts owner 报告显示的零件所有者

#### 代码

```cpp
    /*HEAD REPORT_DISPLAYED_PARTS_OWNER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据您提供的原文，我为您翻译如下：

注意：UF_print_syslog是在V18版本中新增的。请只回答翻译，不要添加无关的内容。 */  
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
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        char  
            owner[17] = { "" } ,  
            pspec[MAX_FSPEC_SIZE + 1];  
        tag_t  
            part = UF_PART_ask_display_part();  
        UF_PART_ask_part_name(part, pspec);  
        UF_CALL(uc4564(pspec, 0, (1 << 0)));  
        WRITENZ(uc4603(owner));  
        WRITE_S(pspec);  
        WRITE_S(owner);  
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

> 根据代码内容，这是一段基于NX Open C++ API实现的NX二次开发代码，主要用于在NX中显示零件信息。以下是代码的主要功能和特点：
>
> 1. 包含头文件，用于调用NX Open C++ API的函数，如uf.h、uf_ui.h、uf_part.h等。
> 2. 定义了一个错误报告函数report_error，用于在函数调用出错时，打印错误信息到系统日志和列表窗口。
> 3. 定义了两个宏WRITE_S和WRITENZ，分别用于向列表窗口写入字符串和整型数据。
> 4. 定义了一个函数write_string_to_listing_window，用于向列表窗口写入字符串数据。
> 5. 定义了一个函数write_integer_to_listing_window，用于向列表窗口写入整型数据。
> 6. 定义了一个函数do_it，用于获取当前显示的零件的名称和所有者信息，并写入列表窗口。
> 7. 定义了ufusr函数，作为NX二次开发的入口函数，在初始化NX Open C++ API后，调用do_it函数，然后终止NX Open C++ API。
> 8. 定义了一个询问卸载函数ufusr_ask_unload，返回立即卸载标志。
>
> 总体来说，这段代码实现了在NX中显示当前零件的名称和所有者信息，并通过错误报告机制和日志窗口记录相关信息，提高了代码的健壮性。
>
