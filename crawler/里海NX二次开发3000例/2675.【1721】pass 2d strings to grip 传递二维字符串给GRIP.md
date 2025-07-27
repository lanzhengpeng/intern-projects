### 【1721】pass 2d strings to grip 传递二维字符串给GRIP

#### 代码

```cpp
    /*HEAD PASS_2D_STRINGS_TO_GRIP CCC UFUN */  
    /*  This program demonstrates that 2 dimensional STRING arrays can be passed  
        to/from GRIP.  The following is the accompanying GRIP code.  The GRIP  
        fills in the first 4 lines, and the ufunc fills in the next 4.  
        number/ n  
        string/ txt(8,15)  
        data/ n, 4, txt, 'line 1', 'line 2', 'line 3', 'line 4', '','','',''  
        grargs/ txt, n  
        PRINT/&NULSTR  
        EJECT/PRINT  
        xspawn/ ufun, 'ufunc/test.sl'  
        PRINT/&NULSTR  
        PRINT/'Back from API:'  
        do/ a10:, ii, 1, n  
    a10:    print/ txt(ii)  
        halt  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是在V18版本中新增的函数。 */  
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
    static void do_it(void)  
    {  
        int  
            ii;  
        char  
            txt[8][16];  
        double  
            n;  
        UF_args_t  
            grip_arg_list[2];  
        grip_arg_list[0].type    = UF_TYPE_CHAR_ARRAY;  
        grip_arg_list[0].length  = 8;  
        grip_arg_list[0].address = txt;  
        grip_arg_list[1].type    = UF_TYPE_DOUBLE;  
        grip_arg_list[1].length  = 0;  
        grip_arg_list[1].address = &n;  
        UF_CALL(UF_ask_grip_args(2, grip_arg_list));  
        WRITE("In from GRIP:\n");  
        for (ii=0; ii<n; ii++)  
        {  
            WRITE_S(txt[ii]);  
        }  
        for (ii=n; ii<8; ii++) sprintf(&txt[ii][0], "line %d", ii+1);  
        n = 8;  
        UF_CALL(UF_set_grip_args(2, grip_arg_list));  
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

> 这段代码是NX的二次开发示例，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在函数调用出错时打印错误信息。
> 2. 定义了WRITE和WRITE_S宏，用于向列表窗口打印信息。
> 3. 定义了write_string_to_listing_window函数，用于向列表窗口打印字符串。
> 4. 定义了do_it函数，用于实现主要逻辑：调用UF_ask_grip_args获取GRIP传入的参数，并打印。修改参数，并调用UF_set_grip_args设置返回参数。
> 5. 调用UF_ask_grip_args获取GRIP传入的参数，并打印。
> 6. 修改参数，并调用UF_set_grip_args设置返回参数。
> 7. 定义了ufusr函数，作为用户函数的入口，初始化并调用do_it，最后终止。
> 8. 定义了ufusr_ask_unload函数，用于卸载用户函数。
>
> 总体来说，这段代码展示了如何在NX中调用GRIP接口，实现与GRIP的数据交互，包括传入传出二维字符串数组。
>
