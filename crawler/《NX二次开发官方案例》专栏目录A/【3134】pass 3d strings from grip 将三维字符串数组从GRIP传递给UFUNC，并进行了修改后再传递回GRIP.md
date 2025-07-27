### 【3134】pass 3d strings from grip 将三维字符串数组从GRIP传递给UFUNC，并进行了修改后再传递回GRIP

#### 代码

```cpp
    /*HEAD PASS_3D_STRINGS_FROM_GRIP CCC UFUN */  
    /*  This program demonstrates that 3 dimensional STRING arrays can be passed  
        to/from GRIP.  The following is the accompanying GRIP code.  The GRIP  
        fills in the first 4 lines, and the ufunc fills in the next 4.  
        number/ n  
        string/ txt(10,3,15)  
        data/ n, 4, $  
              txt, 'line 1,1', 'line 1,2', 'line 1,3', $  
                   'line 2,1', 'line 2,2', 'line 2,3', $  
                   'line 3,1', 'line 3,2', 'line 3,3', $  
                   'line 4,1', 'line 4,2', 'line 4,3', $  
                   '','','', $  
                   '','','', $  
                   '','','', $  
                   '','','', $  
                   '','','', $  
                   '','',''  
        grargs/ txt, n  
        xspawn/ ufun, '/users/webstera/ufunc/junk.sl'  
        do/ a10:, i, 1, n  
    a10:    print/ txt(i,1..3)  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中新增的功能，用于打印系统日志。 */  
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
    static void do_it(void)  
    {  
        int  
            ii, jj;  
        char  
            txt[10][3][16];  
        double  
            n;  
        UF_args_t  
            grip_arg_list[2];  
        grip_arg_list[0].type    = UF_TYPE_CHAR_ARRAY;  
        grip_arg_list[0].length  = 30;  
        grip_arg_list[0].address = txt;  
        grip_arg_list[1].type    = UF_TYPE_DOUBLE;  
        grip_arg_list[1].length  = 0;  
        grip_arg_list[1].address = &n;  
        UF_CALL(UF_ask_grip_args(2, grip_arg_list));  
        for (ii=0; ii<n; ii++)  
            for (jj=0; jj<3; jj++) printf("%d,%d - %s\n", ii, jj, txt[ii][jj]);  
        for (ii=n; ii<2*n; ii++)  
            for (jj=0; jj<3; jj++) sprintf(txt[ii][jj], "line %d,%d", ii+1, jj+1);  
        n = 2 * n;  
        UF_CALL(UF_set_grip_args(2, grip_arg_list));  
    }  
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

> 这段代码是一个NX的二次开发UF程序，其主要功能是实现与GRIP的交互，并演示如何传递三维字符串数组。
>
> 代码的主要逻辑包括：
>
> 1. 定义一个三维字符数组txt，用于存储字符串。
> 2. 通过UF_ask_grip_args函数从GRIP获取参数，其中第一个参数是一个长度为30的三维字符数组，第二个参数是一个双精度浮点数。
> 3. 打印出从GRIP获取的字符串数组的内容。
> 4. 对剩余的字符串数组元素进行初始化，格式为"line x,y"，其中x和y分别表示行号和列号。
> 5. 将修改后的字符串数组作为输出参数设置回GRIP。
> 6. 定义了错误处理函数report_error，用于在UF函数调用失败时打印错误信息。
> 7. 定义了主函数do_it，用于执行上述逻辑。
> 8. 定义了UF入口函数ufusr，初始化后调用do_it，然后终止。
> 9. 定义了卸载函数ufusr_ask_unload，要求立即卸载该UF程序。
>
> 综上所述，该代码通过UF接口与GRIP交互，展示了如何传递和修改三维字符串数组，为NX的二次开发提供了实用的示例。
>
