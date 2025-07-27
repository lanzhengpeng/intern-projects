### 【0873】create scenario section 创建场景部分

#### 代码

```cpp
    /*HEAD CREATE_SCENARIO_SECTION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_sf.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = %d\n", title, n);  
        ECHO(msg);  
    }  
    static void do_it(void)  
    {  
        double  
            property_values[1] = { 10.0 };  
        char  
            name_str[13] = { "SECTION_001" },  
            *offsets[2] = { "0", "0" };  
        tag_t  
            section_tag = NULL_TAG;  
        if (UF_SF_is_scenario_part()) return;  /* illogical TRUE = NOT 译:illogical TRUE = NOT翻译为：不合逻辑的真等于非 */  
    /*  The 2nd argument must be a modifiable char array - see PR 2108747 译:译文：第二个参数必须是一个可修改的字符数组 - 参见PR 2108747。 */  
        UF_CALL(UF_SF_create_section(UF_SF_SECTION_SOCYL, name_str,  
            property_values, offsets, NULL, 0, NULL, &section_tag));  
        WRITE_D(section_tag);  
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

> 这段代码是一个NX二次开发示例，其主要功能是创建一个截面。以下是代码的主要部分介绍：
>
> 1. 头文件包含：代码包含了必要的NX二次开发头文件，如uf.h、uf_ui.h、uf_sf.h等。
> 2. 宏定义：定义了ECHO和UF_CALL两个宏，用于打印日志信息和调用NX函数。
> 3. 错误报告函数：report_error函数用于在调用NX函数出错时打印错误信息。
> 4. 写入整数函数：write_integer_to_listing_window函数用于将整数写入日志窗口。
> 5. 创建截面函数：do_it函数用于创建一个截面，包括设置截面类型、名称、属性值等信息，并调用UF_SF_create_section函数创建截面。
> 6. ufusr函数：这是NX二次开发的入口函数，用于初始化NX系统、调用do_it函数创建截面，然后终止NX系统。
> 7. 卸载询问函数：ufusr_ask_unload函数返回立即卸载二次开发的标志。
>
> 综上所述，这段代码通过NX二次开发接口实现了一个创建截面的功能，包括错误处理和日志输出。
>
