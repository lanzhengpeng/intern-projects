### 【1724】pass udobj class id from ufsta 将UDO对象类ID从UFSTA传递

#### 代码

```cpp
    /*HEAD PASS_UDOBJ_CLASS_ID_FROM_UFSTA CCC UFUN */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_udobj.h>  
    #include <uf_ui.h>  
    #include <uf_exit.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n*** ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
        }  
        return(irc);  
    }  
    static UF_UDOBJ_class_t MY_class_id ;  
    DllExport extern UF_UDOBJ_class_t get_my_class_id(void)  
    {  
        return MY_class_id;  
    }  
    static void do_it(void)  
    {  
        char  
            msg[133];  
        UF_CALL(UF_UDOBJ_create_class("MY_test", "my UDOs", &MY_class_id));  
        UF_CALL(UF_UI_add_to_class_sel(MY_class_id));  
        sprintf(msg, "In ufsta get_my_class_id() = %d\n", get_my_class_id());  
        UF_print_syslog(msg, FALSE);  
    }  
    /* qq3123197280 */  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }

```

#### 代码解析

> 这段代码是NX二次开发的一个示例，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于打印出函数调用失败的信息。
> 2. 声明了一个全局变量MY_class_id，用于存储创建的UDO类ID。
> 3. 定义了一个函数get_my_class_id，用于获取全局变量MY_class_id的值。
> 4. 定义了一个函数do_it，在其中调用UF_UDOBJ_create_class创建了名为"MY_test"的UDO类，并调用UF_UI_add_to_class_sel将该类添加到NX的类选择器中。
> 5. 定义了ufsta函数，这是NX二次开发的入口函数。在其中调用UF_initialize初始化NX环境，调用do_it执行操作，最后调用UF_terminate终止NX环境。
> 6. 使用#define UF_CALL(X)宏将NX函数调用封装到report_error中，便于错误处理。
> 7. 使用UF_print_syslog函数打印信息到NX日志。
>
> 总的来说，这段代码通过NX提供的API实现了创建一个UDO类，并将其添加到NX的类选择器中。通过NX二次开发的机制，可以方便地扩展NX的功能。
>
