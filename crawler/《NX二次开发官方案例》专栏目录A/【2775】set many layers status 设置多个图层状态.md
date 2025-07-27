### 【2775】set many layers status 设置多个图层状态

#### 代码

```cpp
    /*HEAD SET_MANY_LAYERS_STATUS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_layer.h>  
    #include <uf_view.h>  
    #include <uf_layout.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的函数，用于打印系统日志信息。 */  
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
        UF_LAYER_status_info_t  
            changes[5];  
        changes[0].layer_number = 1;  
        changes[0].layer_status = UF_LAYER_ACTIVE_LAYER;  
        changes[1].layer_number = 2;  
        changes[1].layer_status = UF_LAYER_ACTIVE_LAYER;  
        changes[2].layer_number = 3;  
        changes[2].layer_status = UF_LAYER_ACTIVE_LAYER;  
        changes[3].layer_number = 4;  
        changes[3].layer_status = UF_LAYER_ACTIVE_LAYER;  
        changes[4].layer_number = 5;  
        changes[4].layer_status = UF_LAYER_WORK_LAYER;  
    /*  Work around for PR4108411 UF_LAYER_set_many_layers_status  cannot set the work layer to a layer which is currently inactive.   
        UF_CALL( UF_LAYER_set_status( 5, UF_LAYER_ACTIVE_LAYER ));  
    */  
        UF_CALL( UF_LAYER_set_many_layers_status( 5, changes ));  
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

> 这段代码是一个NX的二次开发示例，其主要功能是设置多个图层的激活状态。代码中使用了UF函数库，包含以下关键点：
>
> 1. 包含必要的头文件，如uf.h、uf_ui.h等，用于调用UF函数。
> 2. 定义了ECHO和UF_CALL宏，分别用于打印变量值和调用UF函数并报告错误。
> 3. 实现了report_error函数，用于打印错误信息。
> 4. 在do_it函数中，定义了一个数组changes，存储要设置图层状态的信息，包括图层号和状态。然后调用UF_LAYER_set_many_layers_status函数批量设置这些图层的状态。
> 5. 实现了ufusr函数，这是NX二次开发的入口函数。在该函数中，首先调用UF_initialize初始化UF，然后调用do_it执行设置图层状态的操作，最后调用UF_terminate终止UF。
> 6. 实现了ufusr_ask_unload函数，用于在卸载时立即终止UF。
>
> 总的来说，这段代码通过UF函数库实现了批量设置NX图层状态的功能，展示了NX二次开发的基本流程。
>
