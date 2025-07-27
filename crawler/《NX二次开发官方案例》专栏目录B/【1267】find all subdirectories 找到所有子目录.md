### 【1267】find all subdirectories 找到所有子目录

#### 代码

```cpp
    /*HEAD FIND_ALL_SUBDIRECTORIES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的函数。

仅回答译文，不要添加任何废话。 */  
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
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
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
        int   
            inx = 0,  
            num_found = 0;  
        char   
            **path;  
        UF_CALL( UF_find_all_subdirectories( "startup", &num_found, &path ));  
        WRITE_D( num_found );   
        for( inx = 0; inx < num_found; inx++ )  
        {  
            WRITE_S( path[inx] );  
        }     
        UF_free_string_array( num_found, path );         
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

> 这段代码是NX Open的UF函数调用示例，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于输出错误信息到系统日志和控制台窗口。
> 2. 定义了整数输出函数write_integer_to_listing_window和字符串输出函数write_string_to_listing_window，用于输出信息到列表窗口。
> 3. 主函数do_it中，使用UF_find_all_subdirectories函数查找指定目录下的所有子目录，并输出找到的子目录数量和名称。
> 4. ufusr函数是NX Open的入口函数，在这里调用UF_initialize初始化，调用do_it执行主逻辑，最后调用UF_terminate终止。
> 5. ufusr_ask_unload函数返回立即卸载标志，表示该库可以被立即卸载。
>
> 总体来说，这是一个简单的NX Open二次开发示例，演示了UF函数的调用、错误处理和输出信息到控制台/列表窗口的方法。
>
