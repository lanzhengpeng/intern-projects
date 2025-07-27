### 【0521】convert part tag to leaf name and back 将部件的标签转换为叶名称，然后再转换回标签

#### 代码

```cpp
    /*HEAD CONVERT_PART_TAG_TO_LEAF_NAME_AND_BACK CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog是V18版本中新增的函数。 */  
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
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    static void do_it(void)  
    {  
        logical  
            nxman;  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            full_name[MAX_FSPEC_SIZE+1],  
            leaf_name[MAX_FSPEC_SIZE+1];  
        WRITE_D(part);  
        UF_CALL(UF_PART_ask_part_name(part, full_name));  
        WRITE_S(full_name);  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        if (nxman)  
            UF_CALL(UF_PART_name_for_display(full_name, leaf_name));  
        else  
            UF_CALL(uc4574(full_name, 2, leaf_name));  
        WRITE_S(leaf_name);  
        UF_CALL(UF_PART_ask_tag_of_disp_name(leaf_name, &part));  
        WRITE_D(part);  
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

> 这段代码是NX的二次开发代码，其主要功能是将NX中的部件tag转换为部件的叶节点名称，然后再转换回tag。
>
> 主要功能包括：
>
> 1. 错误处理：通过宏UF_CALL()来调用NX API函数，并使用report_error()函数来处理错误。
> 2. 日志输出：定义了宏WRITE_D()和WRITE_S()，用于向日志窗口输出整型和字符串数据。
> 3. 主函数：在do_it()函数中，首先获取当前显示的部件的tag，然后调用UF_PART_ask_part_name()获取其全名。接着判断NX Manager是否激活，并调用相应函数将全名转换为叶节点名称。最后，调用UF_PART_ask_tag_of_disp_name()将叶节点名称转换回tag。
> 4. NX初始化和终止：在ufusr()函数中，首先初始化NX，然后调用do_it()，最后终止NX。
> 5. 卸载请求：在ufusr_ask_unload()函数中，返回立即卸载标志。
>
> 总的来说，这段代码实现了NX部件tag和叶节点名称之间的相互转换，并具有错误处理和日志输出功能。
>
