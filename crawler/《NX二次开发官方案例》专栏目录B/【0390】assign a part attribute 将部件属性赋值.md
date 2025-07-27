### 【0390】assign a part attribute 将部件属性赋值

#### 代码

```cpp
    /*HEAD ASSIGN_A_PART_ATTRIBUTE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_attr.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译如下：

注意：UF_print_syslog是V18版本新增的函数，请只回答翻译内容，无需其他赘述。 */  
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
            part = UF_PART_ask_display_part();  
        char  
            str[UF_ATTR_MAX_STRING_LEN+1];  
        UF_ATTR_value_t  
            value;  
        value.type = UF_ATTR_string;  
    /*  Either set the pointer to point to the declared character space 译:翻译为：将指针设置为指向声明过的字符空间。 */  
        value.value.string = str;  
        strcpy(value.value.string, "This is the copied string value");  
        UF_CALL(UF_ATTR_assign(part, "A_Title", value));  
    /*  Or use a string literal 译:使用字符串字面量 */  
        value.value.string = "A literal string value";  
        UF_CALL(UF_ATTR_assign(part, "B_Title", value));  
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

> 这段代码是一个NX的二次开发程序，主要实现以下功能：
>
> 1. 错误报告函数：使用宏UF_CALL来封装NX的函数调用，并在调用失败时通过report_error函数报告错误。
> 2. 设置零件属性：使用UF_PART_ask_display_part获取当前显示的零件，然后使用UF_ATTR_assign函数给零件设置属性，包括通过字符串变量设置属性值，以及直接通过字符串字面量设置属性值。
> 3. 初始化和终止：在ufusr函数中初始化NX环境，执行设置属性的操作，并在结束时终止NX环境。
> 4. 卸载标志：通过ufusr_ask_unload函数返回立即卸载标志。
>
> 总的来说，这段代码实现了在NX中给当前显示的零件设置属性的功能，并包含了错误处理和NX环境的初始化与终止。
>
