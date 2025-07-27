### 【1827】read value of part attribute 读取零件属性值

#### 代码

```cpp
    /*HEAD READ_VALUE_OF_PART_ATTRIBUTE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的提示，UF_print_syslog是在V18版本中新增的函数。因此，只需要回答翻译如下：

UF_print_syslog是V18版本中新增的函数。 */  
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
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
         char  
             title[UF_ATTR_MAX_TITLE_LEN+1] = { "" },  
             msg[UF_ATTR_MAX_STRING_LEN+1],  
             str[UF_ATTR_MAX_STRING_LEN+1];  
        tag_t  
            part = UF_PART_ask_display_part();  
        UF_ATTR_value_t  
            value;  
    /*  Reading a UF_ATTR_string so assign the character array to the pointer 里海译:翻译：读取一个 UF_ATTR_string，因此将字符数组赋值给指针。 */  
        value.value.string = str;  
        while (prompt_for_text("Enter attribute title", title))  
        {  
            if (!UF_CALL(UF_ATTR_read_value(part, title, UF_ATTR_string, &value)))  
            {  
                sprintf(msg, "\n%s = %s\n", title, value.value.string);  
                strcpy(value.value.string, "");  
            }  
            if (!UF_UI_open_listing_window())  
            {  
               UF_UI_write_listing_window(msg);  
            }  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是NX二次开发中用于读取零件属性值的示例代码。主要功能包括：
>
> 1. 定义错误报告函数report_error，用于输出函数执行错误信息。
> 2. 定义提示输入文本函数prompt_for_text，用于获取用户输入的属性标题。
> 3. 定义执行函数do_it，用于读取并输出零件的属性值。它通过循环提示用户输入属性标题，然后调用UF_ATTR_read_value函数读取对应属性的值，并输出。在读取时，将字符串数组赋值给UF_ATTR_value_t结构体的指针。
> 4. 定义ufusr函数，作为NX二次开发的入口函数。在初始化NX环境后，调用do_it函数执行读取操作，最后终止NX环境。
> 5. 定义卸载函数ufusr_ask_unload，用于在二次开发模块卸载时立即卸载。
>
> 总的来说，这段代码实现了通过用户输入属性标题，读取并输出零件属性值的功能。通过调用NX提供的UF_ATTR_read_value等函数，实现了与NX系统的交互。
>
