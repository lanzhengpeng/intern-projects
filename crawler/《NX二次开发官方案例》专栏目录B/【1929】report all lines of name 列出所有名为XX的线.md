### 【1929】report all lines of name 列出所有名为XX的线

#### 代码

```cpp
    /*HEAD list_named_line CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的信息，UF_print_syslog 是 V18 版本中新增的函数。 */  
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
    /* qq3123197280 */  
    static tag_t ask_next_of_name_and_type(tag_t part, char *name, int type,  
        tag_t object)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_by_name_and_type(part, name, type, TRUE,  
            &object))) return (object);  
        return (NULL_TAG);  
    }  
    /* qq3123197280 */  
    static void list_named_line(char *line_name)  
    {  
        tag_t  
            line = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        int  
            ii = 1;  
        char  
            msg[133];  
        line = ask_next_of_name_and_type(part, line_name,   
                                        UF_line_type, line);  
        if (line != NULL_TAG)   
        {  
            sprintf(msg, "%d. Tag of Line named %s: %d\n", ii, line_name, line);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                ii++;  
            }  
        }  
    }  
    static void do_it(void)  
    {  
        list_named_line("MINOR_DIA");  
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

> 这段NX二次开发代码的主要功能是列出当前显示部件中名为"MINOR_DIA"的线的tag号。
>
> 代码的主要组成部分包括：
>
> 1. 引入NX Open的UF头文件，这些文件提供了访问NX功能的接口。
> 2. 定义了一个报告错误函数report_error，用于在调用NX Open接口失败时打印错误信息。
> 3. 定义了一个ask_next_of_name_and_type函数，用于按名称和类型循环查找对象，并返回下一个对象的tag。
> 4. 定义了list_named_line函数，用于列出当前显示部件中指定名称线的tag号。
> 5. 定义了一个do_it函数，该函数调用list_named_line来列出名为"MINOR_DIA"的线的tag号。
> 6. 定义了ufusr函数，这是NX二次开发的入口函数。在该函数中初始化NX Open，调用do_it函数，然后终止NX Open。
> 7. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示二次开发应用程序不需要在NX关闭时等待。
>
> 总的来说，这段代码实现了按名称查找并打印线的tag号的功能，通过NX Open提供的接口来实现对NX模型的访问。
>
