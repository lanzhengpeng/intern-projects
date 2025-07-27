### 【2230】report number of out of date wave objects 报告过时的波形对象数量

#### 代码

```cpp
    /*HEAD REPORT_NUMBER_OF_OUT_OF_DATE_WAVE_OBJECTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_wave.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的函数，用于在UF系统中打印系统日志信息。 */  
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
    static void do_it(void)  
    {  
        int  
            n_objects;  
        tag_t  
            *objects,  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_WAVE_ask_out_of_date_objects(part, &n_objects, &objects));  
        WRITE_D(n_objects);  
        if (n_objects > 0) UF_free(objects);  
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

> 这段代码是NX Open C++ API编写的二次开发代码，主要用于检测NX零件中过时的波形面。
>
> 代码的主要功能包括：
>
> 1. 定义了宏函数UF_CALL和WRITE_D，用于调用NX API函数并报告错误，以及将整数写入列表窗口。
> 2. 定义了report_error函数，用于在发生API调用错误时打印错误信息。
> 3. 定义了write_integer_to_listing_window函数，用于将整数写入列表窗口。
> 4. 定义了do_it函数，该函数调用UF_WAVE_ask_out_of_date_objectsAPI函数，获取零件中过时的波形面数量和对象数组，并写入列表窗口。
> 5. ufusr函数是NX二次开发的标准入口函数，在调用UF_initialize初始化API后，执行do_it函数，最后调用UF_terminate终止API。
> 6. ufusr_ask_unload函数返回UF_UNLOAD_IMMEDIATELY，表示立即卸载二次开发程序。
>
> 总体来说，这段代码实现了检测NX零件中过时的波形面数量，并将结果打印到列表窗口的功能，为用户提供了一种检测零件中过时波形面的方法。
>
