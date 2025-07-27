### 【1004】delete all clearance data sets 删除所有间隙数据集

#### 代码

```cpp
    /*HEAD DELETE_ALL_CLEARANCE_DATA_SETS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_clear.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是 V18 版本中新增的函数，用于打印系统日志信息。 */  
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
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_CLEAR_delete_all(part));  
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

> 根据代码内容，这是一段NX Open C++ API的二次开发代码，主要用于删除当前显示的部件中的所有清除数据集。
>
> 主要功能如下：
>
> 1. 包含必要的头文件，如stdio.h、string.h、uf.h、uf_ui.h、uf_clear.h。
> 2. 定义了UF_CALL宏，用于调用NX Open C++ API函数，并在出错时报告错误信息。
> 3. 定义了report_error函数，用于在出错时打印错误信息。
> 4. 定义了do_it函数，用于获取当前显示的部件，并调用UF_CLEAR_delete_all函数删除其中的所有清除数据集。
> 5. 定义了ufusr函数，用于初始化和终止NX Open C++ API，并调用do_it函数执行删除操作。
> 6. 定义了ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY，表示在卸载UF时立即卸载。
>
> 综上，这段代码的主要功能是删除当前显示的NX部件中的所有清除数据集，以清理部件。通过NX Open C++ API实现二次开发，并在出错时提供错误报告机制。
>
