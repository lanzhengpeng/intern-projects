### 【1979】report all transferable types 报告所有可传输类型

#### 代码

```cpp
    /*HEAD REPORT_ALL_TRANSFERABLE_TYPES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据上下文，UF_print_syslog 是 V18 新增的功能，请不要添加任何废话，只回答翻译。 */  
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
        logical  
            flag;  
        int  
            n;  
        char  
            *base_dir,  
            msg[300];  
        UF_CALL(UF_translate_variable("UGII_BASE_DIR", &base_dir));  
        UF_CALL(UF_UI_open_listing_window());  
        for (n = 1; n < UF_max_entity_type; n++)  
        {  
            UF_CALL(UF_OBJ_is_type_transferable(n, &flag));  
            if (flag)  
            {  
                sprintf(msg, "type %d is transferable\n", n);  
                UF_CALL(UF_UI_write_listing_window(msg));  
    #ifdef _WIN32  
                sprintf(msg, "findstr /B /E /C:\"#define .*_type *%d\" %s%s\n",  
                    n, base_dir, "\\ugopen\\uf_object_types.h");  
    #else  
                sprintf(msg, "grep \"^#define .*_type *%d[[:space:]]\" %s%s\n",  
                    n, base_dir, "/ugopen/uf_object_types.h");  
    #endif  
                system(msg);  
            }  
        }  
    }  
    /* qq3123197280 */  
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

> 根据代码，这是一段NX Open C++ API编写的NX二次开发代码，主要功能是遍历所有NX对象类型，并判断哪些是可传输的。代码的主要步骤如下：
>
> 1. 包含必要的头文件，如uf.h, uf_ui.h等，以使用NX Open C++ API的函数。
> 2. 定义宏UF_CALL，用于封装UF函数调用，并在调用失败时报告错误。
> 3. 定义报告错误的函数report_error，用于打印错误信息。
> 4. 定义执行主体逻辑的函数do_it。
> 5. 在do_it函数中，首先获取NX的系统变量UGII_BASE_DIR，以获取NX的安装目录。
> 6. 打开列表窗口，用于显示结果。
> 7. 遍历所有对象类型(从1到UF_max_entity_type-1)，使用UF_OBJ_is_type_transferable函数判断每个类型是否可传输。
> 8. 如果类型可传输，则打印类型号到列表窗口，并根据不同平台，构造查找该类型定义的命令，并执行。在Windows平台使用findstr，在Linux平台使用grep。
> 9. 在ufusr函数中，初始化NX Open C++ API，调用do_it函数，然后终止NX Open C++ API。
> 10. 定义ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY，表示该二次开发代码可以立即卸载。
>
> 总的来说，这段代码利用NX Open C++ API遍历了所有对象类型，并判断哪些是可传输的，同时提供了跨平台的方法来查找每个类型的定义，是一个典型的NX二次开发示例。
>
