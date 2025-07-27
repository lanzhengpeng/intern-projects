### 【2944】swap all components to entire part 将所有组件交换到整个部件

#### 代码

```cpp
    /*HEAD SWAP_ALL_COMPONENTS_TO_ENTIRE_PART CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_undo.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中的新增功能，用于打印系统日志。 */  
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
    static void swap_all_components_reference_set(char *refset)  
    {  
        tag_t  
            cset,  
            part = UF_ASSEM_ask_work_part();  
        UF_CALL(UF_ASSEM_ask_all_comp_cset(part, &cset));  
        UF_CALL(UF_ASSEM_set_ref_set_by_cset(cset, refset));  
    }  
    static void do_it(void)  
    {  
        UF_UNDO_mark_id_t  
            mark;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, "Swap All to Entire Part", &mark));  
        swap_all_components_reference_set("None");  
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

> 这段代码是用于NX的二次开发，其主要功能是交换所有组件到整个部件的引用集。
>
> 主要思路是：
>
> 1. 包含了NX的UF库头文件。
> 2. 定义了错误报告函数report_error，用于打印出函数调用失败的错误码和错误信息。
> 3. 定义了swap_all_components_reference_set函数，用于交换所有组件到指定的引用集。该函数首先获取当前工作部件的所有组件集合，然后设置该组件集合的引用集为指定字符串。
> 4. 定义了do_it函数，用于执行交换操作。该函数首先设置一个undo标记，然后调用swap_all_components_reference_set函数将所有组件的引用集设置为"None"。
> 5. 定义了ufusr函数，这是NX二次开发的标准入口函数。在函数中，首先初始化NX系统，然后调用do_it函数执行交换操作，最后终止NX系统。
> 6. 定义了ufusr_ask_unload函数，返回立即卸载，表示该二次开发模块不需要在NX中持续加载。
>
> 总体来说，这段代码通过二次开发实现了NX中所有组件到整个部件的引用集的交换操作，提高了NX的操作效率。
>
