### 【2956】test for existence of specified part attribute 检查指定零件属性是否存在

#### 代码

```cpp
    /*HEAD TEST_FOR_EXISTENCE_OF_SPECIFIED_PART_ATTRIBUTE CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据注释，UF_print_syslog 是 V18 中新增的函数。 */  
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
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static logical prompt_for_text_item(char *prompt, char *item, char *text)  
    {  
        int  
            ia[1],  
            ip[1] = { 301 },  
            resp;  
        double  
            da[1];  
        char  
            ca[1][31],  
            menu[1][16];  
        strncpy(&menu[0][0], item, 15);  
        menu[0][16] = '\0';  
        strncpy(&ca[0][0], text, 30);  
        ca[0][31] = '\0';  
        resp = uc1613(prompt, menu, 1, ia, da, ca, ip);  
        if (resp == 3 || resp == 4)  
        {  
            if (!strcmp(&ca[0][0], text)) return FALSE;  
            strcpy(text, &ca[0][0]);  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static logical attribute_exists(tag_t object, char *title)  
    {  
        int  
            type;  
        UF_CALL(UF_ATTR_find_attribute(object, UF_ATTR_any, title, &type));  
        if (type == 0) return FALSE;  
        return TRUE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            displayed_part = UF_PART_ask_display_part();  
        char  
            title[UF_ATTR_MAX_TITLE_LEN+1] = { "" };  
        while (prompt_for_text_item("Check attribute existence", "Title", title))  
        {  
            WRITE_S(title);  
            WRITE_L(attribute_exists(displayed_part, title));  
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

> 这段代码是一个NX Open C++ UFUN，用于检查显示部件是否具有指定的属性。以下是代码的主要功能和流程：
>
> 1. 包含了NX Open C++的头文件，用于访问UF、UI和ATTR模块的功能。
> 2. 定义了UF_CALL宏，用于调用UF函数并报告错误。如果UF函数返回错误码，会记录错误信息。
> 3. 定义了WRITE_S和WRITE_L宏，用于将字符串和逻辑值输出到列表窗口。
> 4. 定义了prompt_for_text_item函数，用于提示用户输入属性标题，并返回修改后的标题。
> 5. 定义了attribute_exists函数，用于检查指定对象是否存在指定标题的属性。
> 6. 定义了do_it函数，用于主流程。首先获取当前显示的部件，然后循环提示用户输入属性标题，并检查是否存在该属性，将结果输出到列表窗口。
> 7. ufusr函数是UFUN的入口点。首先初始化UF模块，然后调用do_it函数，最后终止UF模块。
> 8. ufusr_ask_unload函数用于卸载UFUN，返回立即卸载。
>
> 总体来说，这段代码的主要功能是提供一个交互式的界面，让用户可以检查当前显示部件是否具有指定的属性，并将结果输出到列表窗口。代码结构清晰，易于理解和使用。
>
