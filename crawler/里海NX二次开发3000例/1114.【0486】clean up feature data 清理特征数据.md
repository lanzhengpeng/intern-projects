### 【0486】clean up feature data 清理特征数据

#### 代码

```cpp
    /*HEAD CLEAN_UP_FEATURE_DATA CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据注释，UF_print_syslog 是在 V18 中新增的。 */  
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
        UF_CALL(UF_PART_cleanup( UF_PART_cleanup_feature ));  
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

> 这段代码是NX Open C++的一个示例，用于清理NX零件的特征数据。以下是代码的主要功能：
>
> 1. 包含必要的NX Open C++头文件，用于进行错误处理、用户界面操作和零件操作。
> 2. 定义了错误报告函数report_error，用于输出错误信息到系统日志和控制台。
> 3. 定义了清理函数do_it，调用UF_PART_cleanup来清理零件的特征数据。
> 4. 定义了ufusr函数，这是NX二次开发的入口函数。在ufusr中，初始化NX Open，调用do_it进行清理，然后终止NX Open。
> 5. 定义了卸载函数ufusr_ask_unload，返回立即卸载标志。
>
> 总的来说，这段代码的主要作用是清理NX零件的特征数据。在NX Open C++中进行二次开发时，通常会使用类似的代码结构，包括错误处理、函数调用和初始化/终止。
>
