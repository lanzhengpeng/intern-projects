### 【0146】add temp directory to kf search path 将临时目录添加到知识框架搜索路径

#### 代码

```cpp
    /*HEAD ADD_TEMP_DIRECTORY_TO_KF_SEARCH_PATH CCC UFUN */  
    /*  
        Note:  The function UF_KF_add_search_library does not work!  
        See PR 4260116.  
    */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_kf.h>  
    #define ECHOS(X)   (printf("%s = %s\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog 是在 V18 版本中新增的功能，专门用于打印系统日志。 */  
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
        char  
            *t_dir;  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &t_dir));  
        ECHOS(t_dir);  
        UF_CALL(UF_KF_init_part(part));  
        UF_CALL(UF_KF_add_search_library(t_dir));  
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

> 根据提供的代码，这是NX Open C++ API的二次开发代码。以下是代码的主要内容和功能：
>
> 1. 引入了NX Open C++ API的相关头文件，包括uf.h, uf_ui.h, uf_part.h, uf_kf.h等。
> 2. 定义了宏ECHOS和UF_CALL，用于打印变量值和调用NX API函数，并在出错时打印错误信息。
> 3. 实现了report_error函数，用于在调用NX API出错时打印错误信息。
> 4. 定义了do_it函数，用于添加临时目录到知识融合的搜索路径。具体包括：获取当前显示的部件获取环境变量UGII_TMP_DIR的值，即临时目录路径初始化部件的知识融合数据将临时目录添加到知识融合的搜索路径
> 5. 获取当前显示的部件
> 6. 获取环境变量UGII_TMP_DIR的值，即临时目录路径
> 7. 初始化部件的知识融合数据
> 8. 将临时目录添加到知识融合的搜索路径
> 9. 实现了ufusr函数，这是NX二次开发的标准入口函数。在ufusr中，先初始化NX环境，然后调用do_it函数，最后终止NX环境。
> 10. 实现了ufusr_ask_unload函数，用于卸载二次开发应用程序。此函数返回UF_UNLOAD_IMMEDIATELY，表示应用程序可以立即卸载。
>
> 总体来说，这段代码的功能是向NX的知识融合搜索路径中添加一个临时目录，以便NX可以在这个目录下查找知识融合相关的文件。这是一个典型的NX二次开发示例，演示了如何使用NX Open C++ API进行基本的NX环境初始化、错误处理、知识融合操作等。
>
