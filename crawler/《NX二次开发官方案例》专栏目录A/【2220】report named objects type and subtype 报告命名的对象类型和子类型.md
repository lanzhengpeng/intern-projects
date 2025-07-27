### 【2220】report named objects type and subtype 报告命名的对象类型和子类型

#### 代码

```cpp
    /*HEAD REPORT_NAMED_OBJECTS_TYPE_AND_SUBTYPE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据提供的信息，翻译为：

UF_print_syslog是V18版本新增的函数。 */  
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
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    static void report_object_type_and_subtype(tag_t object)  
    {  
        int  
            subtype,  
            type;  
        WRITE_D(object);  
        if (object != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            WRITE_D(type);  
            WRITE_D(subtype);  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            object = NULL_TAG;  
        char  
            name[MAX_ENTITY_NAME_SIZE+1] = { "" };  
        while (prompt_for_text("Enter item name", name))  
            while ( !UF_OBJ_cycle_by_name(name, &object) && (object != NULL_TAG))  
                report_object_type_and_subtype(object);  
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

> 这段NX二次开发代码的功能是报告NX对象及其子类型的类型和子类型。以下是代码的主要内容和逻辑：
>
> 1. 头文件和宏定义：引入了必要的NX Open头文件，如uf.h, uf_ui.h, uf_obj.h等。定义了UF_CALL宏，用于报告函数调用错误。定义了WRITE_D宏，用于将整数写入列表窗口。
> 2. 引入了必要的NX Open头文件，如uf.h, uf_ui.h, uf_obj.h等。
> 3. 定义了UF_CALL宏，用于报告函数调用错误。
> 4. 定义了WRITE_D宏，用于将整数写入列表窗口。
> 5. 错误报告函数：report_error函数用于报告函数调用错误，并将错误信息写入系统日志和列表窗口。
> 6. report_error函数用于报告函数调用错误，并将错误信息写入系统日志和列表窗口。
> 7. 提示输入文本函数：prompt_for_text函数用于提示用户输入文本，并返回输入结果。
> 8. prompt_for_text函数用于提示用户输入文本，并返回输入结果。
> 9. 报告对象类型和子类型函数：report_object_type_and_subtype函数用于报告给定NX对象的类型和子类型。
> 10. report_object_type_and_subtype函数用于报告给定NX对象的类型和子类型。
> 11. 主逻辑函数：do_it函数用于循环提示用户输入NX对象名称，然后报告每个对象的类型和子类型。
> 12. do_it函数用于循环提示用户输入NX对象名称，然后报告每个对象的类型和子类型。
> 13. ufusr函数：NX二次开发主函数，用于初始化NX，调用do_it函数，然后终止NX。
> 14. NX二次开发主函数，用于初始化NX，调用do_it函数，然后终止NX。
> 15. 卸载函数：ufusr_ask_unload函数用于定义程序的立即卸载行为。
> 16. ufusr_ask_unload函数用于定义程序的立即卸载行为。
>
> 总体来说，这段代码通过交互方式获取NX对象的名称，并报告每个对象的类型和子类型信息。它利用NX提供的API来实现对象查询、类型获取等功能。代码结构清晰，功能明确。
>
