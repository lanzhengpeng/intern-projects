### 【1719】orient view 面向视图

#### 代码

```cpp
    /*HEAD ORIENT_VIEW CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：注意，UF_print_syslog是在V18版本中新增的，请只回答翻译内容，不要添加其他无关内容。 */  
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
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
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
        tag_t  
            view;  
        char  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        while (prompt_for_text("Enter alignment view name", view_name))  
        {      
            UF_CALL(UF_VIEW_ask_tag_of_view_name(view_name, &view));  
            if (view)  
                WRITENZ(uc6434("", 3, view, NULL));  
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

> 这段NX二次开发代码主要用于查询和打印视图的朝向信息。代码的主要功能包括：
>
> 1. 错误报告函数：定义了report_error函数，用于在发生UF函数调用错误时打印错误信息。
> 2. 整数写入列表窗口：定义了write_integer_to_listing_window函数，用于将整数参数写入NX的列表窗口。
> 3. 提示文本输入：定义了prompt_for_text函数，用于提示用户输入文本并获取输入结果。
> 4. 主函数逻辑：在do_it函数中，循环提示用户输入视图名称，并查询视图的tag，然后打印视图的朝向信息。
> 5. UFusr入口函数：定义了ufusr函数作为NX二次开发的入口函数，调用do_it函数实现主要逻辑。
> 6. 卸载函数：定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 总体而言，该代码实现了通过用户输入视图名称，查询并打印视图朝向信息的功能。
>
