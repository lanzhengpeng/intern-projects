### 【0393】assign part attribute by using tag 将零件属性分配给指定的标签

#### 代码

```cpp
    /*HEAD ASSIGN_A_PART_ATTRIBUTE_BY_USING_PART_TAG CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本新增的函数。 */  
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
            part_tag = UF_PART_ask_display_part();  
        char  
            str[UF_ATTR_MAX_STRING_LEN+1];  
        UF_ATTR_value_t  
            value;  
        int  
            cnt,  
            ii;  
        UF_ATTR_part_attr_t  
            *attrs;  
        printf("\npart_tag = %d\n", part_tag);  
        value.type = UF_ATTR_string;  
        value.value.string = str;  
        strcpy(value.value.string, "This is the string value");  
        UF_CALL(UF_ATTR_assign(part_tag, "A_Title", value));  
        UF_CALL(UF_ATTR_ask_part_attrs(part_tag, &cnt, &attrs));  
        for (ii = 0; ii < cnt; ii++)  
            printf("%s = %s\n", attrs[ii].title, attrs[ii].string_value);  
        UF_free(attrs);  
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

> 这段NX二次开发代码的主要功能是使用零件标签给零件分配一个属性。
>
> 代码的主要步骤包括：
>
> 1. 包含所需的NX Open API头文件，如uf.h、uf_ui.h、uf_attr.h等。
> 2. 定义了一个错误报告函数report_error，用于输出错误信息和调用信息。
> 3. 定义了主体函数do_it，其功能如下：获取当前显示的零件标签part_tag。设置一个字符串值"This is the string value"。使用UF_ATTR_assign函数给零件分配一个名为"A_Title"的字符串属性，值设置为上述字符串。使用UF_ATTR_ask_part_attrs函数查询零件的所有属性，并输出到控制台。释放属性数组的空间。
> 4. 获取当前显示的零件标签part_tag。
> 5. 设置一个字符串值"This is the string value"。
> 6. 使用UF_ATTR_assign函数给零件分配一个名为"A_Title"的字符串属性，值设置为上述字符串。
> 7. 使用UF_ATTR_ask_part_attrs函数查询零件的所有属性，并输出到控制台。
> 8. 释放属性数组的空间。
> 9. 定义了ufusr函数作为二次开发的入口点，在NX启动时调用。在ufusr中初始化NX Open API，调用do_it函数，然后终止NX Open API。
> 10. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示二次开发模块可以立即卸载。
>
> 总的来说，这段代码通过二次开发的方式，实现了给NX中的零件分配一个字符串属性的功能，并查询和输出了所有属性。
>
