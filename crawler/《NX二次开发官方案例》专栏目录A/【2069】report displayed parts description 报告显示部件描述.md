### 【2069】report displayed parts description 报告显示部件描述

#### 代码

```cpp
    /*HEAD REPORT_DISPLAYED_PARTS_DESCRIPTION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog 是 V18 中的新增功能，只需回答翻译，不要添加无关内容。 */  
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
        char  
            darea[132] = "",  
            pspec[MAX_FSPEC_SIZE + 1];  
        tag_t  
            part = UF_PART_ask_display_part();  
        UF_PART_ask_part_name(part, pspec);  
        UF_CALL(uc4564(pspec, 0, (1 << 5)));  
        uc4607(darea);  
        printf("Part: %s  has the following description area:\n %s \n", pspec, darea);  
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

> 这是一段用于NX Open C++ API的二次开发代码，其主要功能是获取当前显示的零件的描述信息并打印输出。以下是代码的主要组成部分：
>
> 1. 包含了必要的NX Open C++ API头文件，如uf.h、uf_ui.h、uf_part.h等。
> 2. 定义了一个错误报告函数report_error，用于在函数调用失败时报告错误信息，包括错误代码、错误位置、调用信息等。
> 3. 定义了一个do_it函数，用于执行获取零件描述信息的操作。主要步骤包括：获取当前显示的零件对象。获取零件名称。调用未定义的uc4564函数获取零件描述信息，并将结果存储在darea数组中。打印输出零件名称和描述信息。
> 4. 获取当前显示的零件对象。
> 5. 获取零件名称。
> 6. 调用未定义的uc4564函数获取零件描述信息，并将结果存储在darea数组中。
> 7. 打印输出零件名称和描述信息。
> 8. 定义了ufusr函数，这是NX二次开发的入口函数。在函数中初始化NX系统，调用do_it函数执行功能，然后终止NX系统。
> 9. 定义了ufusr_ask_unload函数，用于设置NX二次开发模块的卸载方式为立即卸载。
>
> 总的来说，这段代码实现了获取当前显示零件的描述信息并打印输出的功能。需要注意的是，代码中调用了未定义的uc4564和uc4607函数，需要根据实际情况进行替换为有效的函数。另外，错误报告机制可以有效地跟踪和调试问题。
>
