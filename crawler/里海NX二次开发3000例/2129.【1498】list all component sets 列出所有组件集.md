### 【1498】list all component sets 列出所有组件集

#### 代码

```cpp
    /*HEAD LIST_ALL_COMPONENT_SETS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本新增的功能。文档只要求提供译文，不添加任何废话。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static tag_t ask_next_component_set(tag_t part, tag_t cset)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_component_set_type, &cset));  
        return (cset);  
    }  
    static void list_all_csets(void)  
    {  
        tag_t  
            cset = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            cset_name[UF_OBJ_NAME_LEN + 1];  
        WRITE("component sets in part\n");  
        while ((cset = ask_next_component_set(part, cset)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_name(cset, cset_name));  
            WRITE_S(cset_name);  
        }  
    }  
    static void do_it(void)  
    {  
        list_all_csets();  
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

> 根据代码内容，这是一段用于在NX中列出所有组件集的二次开发代码。具体来说：
>
> 1. 该代码使用UF函数进行NX的二次开发，并包含头文件uf.h等。
> 2. 定义了宏REPORT_ERROR，用于打印错误信息。同时定义了宏WRITE和WRITE_S，用于在列表窗口中写入信息。
> 3. 定义了函数ASK_NEXT_COMPONENT_SET，用于遍历获取下一个组件集。
> 4. 定义了函数LIST_ALL_CSETS，用于列出当前部件中的所有组件集。
> 5. 定义了函数DO_IT，作为程序的入口点。
> 6. 定义了函数UFUSR，作为NX二次开发的入口函数。
> 7. 定义了函数UFUSR_ASK_UNLOAD，用于设置卸载方式。
> 8. 代码的总体流程是：初始化NX环境，调用DO_IT函数列出所有组件集，然后结束NX环境。
> 9. DO_IT函数通过调用LIST_ALL_CSETS来实现主要功能，而LIST_ALL_CSETS则通过循环调用ASK_NEXT_COMPONENT_SET来遍历所有组件集，并调用UF_OBJ_ASK_NAME获取组件集名称，然后打印出来。
> 10. 代码使用了面向对象的方式组织，通过定义不同的函数来实现不同的功能。
>
> 总的来说，这段代码实现了在NX中列出当前部件所有组件集名称的功能，并采用了面向对象的设计方法。
>
