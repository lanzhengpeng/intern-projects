### 【1760】print numbers without trailing zeros 去掉末尾的零后打印数字

#### 代码

```cpp
    /*HEAD PRINT_NUMBERS_WITHOUT_TRAILING_ZEROS CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中新增的函数。 */  
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
    static void strip_it(double n, char *s)  
    {  
        int ii;  
        sprintf(s, "%f", n);  
        for (ii = strlen(s) - 1; s[ii] == '0'; ii--) s[ii] = '\0';  
    }  
    static void do_it(void)  
    {  
        double  
            n1 = 95.5000,  
            n2 = 95.0005;  
        char  
            s1[30],  
            s2[30];  
        strip_it(n1, s1);  
        printf("n1 = %s\n", s1);  
        strip_it(n2, s2);  
        printf("n2 = %s\n", s2);  
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

> 这段NX二次开发代码的主要功能是打印数字，并去除数字末尾的零。
>
> 主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于在调用UF函数出错时打印错误信息。
> 2. 定义了一个去除数字末尾零的函数strip_it，该函数将浮点数转换为字符串，然后去除字符串末尾的零。
> 3. do_it函数中定义了两个浮点数n1和n2，分别赋值为95.5000和95.0005，然后调用strip_it函数去除末尾的零，并打印出来。
> 4. ufusr函数是NX二次开发程序的入口函数，初始化UF环境后调用do_it函数，然后终止UF环境。
> 5. ufusr_ask_unload函数返回立即卸载标志，表示二次开发程序在NX关闭时会立即卸载。
>
> 通过这段代码，可以学习NX二次开发的基本流程，以及如何调用UF函数、处理错误、去除字符串末尾零等技巧。这段代码在NX二次开发中具有实际的应用场景。
>
