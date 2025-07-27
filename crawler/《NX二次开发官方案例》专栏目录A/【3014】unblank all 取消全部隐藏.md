### 【3014】unblank all 取消全部隐藏

#### 代码

```cpp
    /*HEAD UNBLANK_ALL CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_layer.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的功能。 */  
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
            object = NULL_TAG;  
        while (!UF_CALL(UF_LAYER_cycle_by_layer(0, &object)) &&  
            (object != NULL_TAG))  
            UF_CALL(UF_OBJ_set_blank_status(object, FALSE));  
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

> 这段代码是NX的二次开发示例，其主要功能是遍历所有层，取消选中层的隐藏状态。以下是该代码的主要特点：
>
> 1. 包含必要的NX头文件，如uf.h、uf_ui.h、uf_layer.h等，以支持NX的函数调用。
> 2. 定义了宏UF_CALL，用于调用NX的函数，并自动处理错误。如果函数调用失败，会打印错误信息。
> 3. 定义了report_error函数，用于打印错误信息。它接受文件名、行号、函数名和返回码作为参数，并输出错误信息到系统日志和列表窗口。
> 4. 定义了do_it函数，用于遍历所有层，并取消选中层的隐藏状态。通过UF_LAYER_cycle_by_layer函数遍历层，然后通过UF_OBJ_set_blank_status函数取消隐藏状态。
> 5. 定义了ufusr函数，作为NX的二次开发入口点。在该函数中，初始化NX环境，调用do_it函数执行主体功能，然后终止NX环境。
> 6. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示二次开发程序可以立即卸载。
>
> 总的来说，这段代码通过遍历层并取消隐藏状态，实现了取消选中层的隐藏状态的功能。其利用了NX提供的二次开发接口，实现了NX层管理功能。
>
