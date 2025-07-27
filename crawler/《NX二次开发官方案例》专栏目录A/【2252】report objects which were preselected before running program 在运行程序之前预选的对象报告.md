### 【2252】report objects which were preselected before running program 在运行程序之前预选的对象报告

#### 代码

```cpp
    /*HEAD REPORT_OBJECTS_WHICH_WERE_PRESELECTED_BEFORE_RUNNING_PROGRAM CCC UFUN */  
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
    static void do_it(void)  
    {  
        int  
            inx = 0,  
            num_objects = 0;  
        tag_t  
            *objects = NULL;  
        UF_CALL( UF_UI_ask_global_sel_object_list( &num_objects, &objects ));  
        if( num_objects > 0 )  
        {  
            WRITE_D( num_objects );  
            for (inx = 0; inx < num_objects; inx++ )  
            {  
                WRITE_D( objects[inx] );  
            }  
            UF_free( objects );  
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

> 这段代码是NX Open C++的一个示例，用于列出在NX会话开始前已预先选择的对象。具体来说：
>
> 1. 包含了NX Open C++的头文件，如uf.h, uf_ui.h等，以使用NX的API。
> 2. 定义了UF_CALL宏，用于调用NX API并报告错误。
> 3. 定义了WRITE和WRITE_D等宏，用于在列表窗口中输出文本和整数。
> 4. do_it函数用于获取并输出预先选择的对象列表。
> 5. ufusr函数是NX Open C++的入口点函数。在这里，它初始化NX环境，调用do_it函数，然后终止NX环境。
> 6. ufusr_ask_unload函数用于在卸载NX Open C++应用程序时立即卸载。
>
> 总体来说，这段代码通过NX Open C++ API获取并输出了会话开始前已预先选择的对象列表。
>
