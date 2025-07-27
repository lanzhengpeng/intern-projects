### 【2964】test undo stack size 测试撤销栈大小

#### 代码

```cpp
    /*HEAD TEST_UNDO_STACK_SIZE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_undo.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，我为您翻译如下：

UF_print_syslog是V18版本中的新功能。 */  
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
    static void init_array(double *array, int n)  
    {  
        int  
            ii;  
        for (ii = 0; ii < n; ii++) array[ii] = 0;  
    }  
    static void do_it(void)  
    {  
        int  
            another_mark,  
            ii = 0,  
            mark_exists,  
            mark_id;  
        UF_CURVE_line_t  
            line_data;  
        tag_t  
            line_tag;  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        init_array(line_data.start_point, 3);  
        init_array(line_data.end_point, 3);  
        line_data.end_point[0] = 1.0;  
        UF_CALL(UF_UNDO_set_mark (UF_UNDO_visible, NULL_TAG, &mark_id));  
        do  
        {  
            sprintf(msg, "Mark %d...", ii++);  
            UF_CALL(UF_UI_set_status(msg));  
            UF_CALL(UF_CURVE_create_line(&line_data, &line_tag));  
            UF_CALL(UF_UNDO_set_mark (UF_UNDO_visible, NULL_TAG, &another_mark));  
            line_data.start_point[1] = ii;  
            line_data.end_point[1] = ii;  
            UF_CALL(UF_UNDO_ask_mark_exist(mark_id, NULL, &mark_exists));  
        } while (mark_exists && ii < 1000);  
    }  
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

> 这段代码是用于NX的二次开发，主要实现了以下功能：
>
> 1. 错误处理机制：通过定义report_error函数，对UF函数调用进行错误检查和处理。一旦调用失败，将记录错误代码和行号，并输出错误信息。
> 2. 初始化数组：通过init_array函数，用于初始化数组中的元素为0。
> 3. 创建线段：do_it函数中，循环创建线段，并设置标记点，直到达到1000个。每次循环中，会设置一个标记点，然后检查上一个标记点是否存在，如果存在则继续循环。
> 4. 用户自定义函数：通过ufusr函数，用于初始化和终止NX，并调用do_it函数执行创建线段和标记点的操作。
> 5. 卸载请求：通过ufusr_ask_unload函数，请求立即卸载二次开发程序。
>
> 综上所述，这段代码主要用于NX中创建大量线段，并在创建过程中设置标记点，以便于后续的撤销操作。通过自定义的错误处理机制，可以更好地管理和处理程序执行中的错误。
>
