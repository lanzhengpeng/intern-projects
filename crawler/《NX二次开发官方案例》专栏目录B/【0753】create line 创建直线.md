### 【0753】create line 创建直线

#### 代码

```cpp
    /*HEAD CREATE_LINE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文中提到的信息，UF_print_syslog是V18版本中新增的函数。 */  
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
    static void vec3_init(double vec[3])  
    {  
        vec[0] = 0.0;  
        vec[1] = 0.0;  
        vec[2] = 0.0;  
    }  
    static void do_it(void)  
    {  
        UF_CURVE_line_t  
            line_data;  
        tag_t  
            line_tag;  
        vec3_init(line_data.start_point);  
        vec3_init(line_data.end_point);  
        line_data.end_point[0] = 1.0;  
        UF_CALL(UF_CURVE_create_line(&line_data, &line_tag));  
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

> 这段代码是一个NX Open C++ API编写的NX二次开发示例代码，主要实现了以下功能：
>
> 1. 错误报告函数：定义了report_error函数，用于报告函数调用时的错误代码和错误信息。
> 2. 向量初始化函数：定义了vec3_init函数，用于初始化一个三维向量。
> 3. 创建直线函数：定义了do_it函数，用于创建一条从原点到(1,0,0)的直线，并输出错误信息。
> 4. UF初始化和终止：在ufusr函数中，首先调用UF_initialize进行UF初始化，然后调用do_it函数创建直线，最后调用UF_terminate终止UF。
> 5. 卸载请求：ufusr_ask_unload函数用于在卸载用户函数时返回立即卸载的标志。
>
> 通过这个简单的示例，展示了如何使用NX Open C++ API进行NX二次开发，包括错误处理、函数调用和数据初始化等基本操作。
>
