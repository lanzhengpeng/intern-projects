### 【0565】create a sketch on absolute xy plane 在绝对XY平面上创建一个草图

#### 代码

```cpp
    /*HEAD CREATE_A_SKETCH_ON_ABSOLUTE_XY_PLANE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_sket.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文中内容，UF_print_syslog是V18版本中新增的功能，用于打印系统日志。 */  
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
        tag_t  
            sketch_id;  
        double  
            abs[9] = { 1,0,0, 0,1,0, 0,0,0 };  
        char  
            name[MAX_ENTITY_NAME_SIZE+1] = { "ABS_SKETCH" };  
        UF_CALL(UF_SKET_initialize_sketch(name, &sketch_id));  
        UF_CALL(UF_SKET_create_sketch(name, 2, abs, NULL, NULL, 1, &sketch_id));  
        UF_CALL(UF_SKET_terminate_sketch());  
    }  
    /* 里海 */  
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

> 这段代码是一个NX Open的UF二次开发示例，主要实现了以下功能：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于打印出函数调用失败的错误信息，包括文件名、行号、函数名和错误码。
> 2. 创建草图函数：定义了一个do_it函数，用于在绝对坐标系上创建一个平面草图。它首先初始化一个草图对象，然后创建一个位于绝对坐标系原点的平面草图，并将其命名为"ABS_SKETCH"。
> 3. 主函数：定义了ufusr主函数，它是NX二次开发的入口函数。在ufusr中，首先调用UF_initialize初始化NX Open，然后调用do_it执行创建草图的逻辑，最后调用UF_terminate终止NX Open。
> 4. 卸载函数：定义了ufusr_ask_unload函数，用于告诉NX二次开发环境在卸载时不需要等待，可以立即卸载。
>
> 综上所述，这段代码通过UF函数实现了在NX中创建一个位于绝对坐标系原点的平面草图的功能，并提供了错误报告和二次开发环境的初始化/终止逻辑。
>
