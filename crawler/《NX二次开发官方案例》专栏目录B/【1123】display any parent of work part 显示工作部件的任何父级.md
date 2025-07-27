### 【1123】display any parent of work part 显示工作部件的任何父级

#### 代码

```cpp
    /*HEAD DISPLAY_ANY_PARENT_OF_WORK_PART CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是在V18版本中新增的功能。

针对问题“只回答译文，不要废话”，我理解您需要的是对文档内容的简单翻译。文档提到UF_print_syslog是在V18版本中新增的功能。 */  
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
    static void display_any_parent(tag_t component)  
    {  
        int  
            n;  
        tag_t  
            *parents;  
        n = UF_ASSEM_where_is_part_used(component, &parents);  
        if (n > 0)  
        {  
            UF_CALL(UF_PART_set_display_part(parents[0]));  
            UF_free(parents);  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            work = UF_ASSEM_ask_work_part();  
        display_any_parent(work);  
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

> 这是一段用于NX二次开发的UFUNC代码，其主要功能是显示工作部件的任意父级部件。下面是代码的简要介绍：
>
> 1. 头文件：包含了UFUNC API的头文件，用于调用UFUNC函数。
> 2. report_error函数：用于在出错时报告错误信息，包括错误码、错误消息、调用函数名等。
> 3. display_any_parent函数：用于获取工作部件的父级部件，并显示第一个父级部件。
> 4. do_it函数：获取当前工作部件，并调用display_any_parent函数显示其父级部件。
> 5. ufusr函数：NX二次开发的入口函数，用于初始化UFUNC环境，调用do_it函数，然后终止UFUNC环境。
> 6. ufusr_ask_unload函数：用于处理卸载二次开发时的情况，返回立即卸载标志。
>
> 通过这段代码，可以实现显示工作部件的任意一个父级部件的功能，有助于在NX中进行二次开发。
>
