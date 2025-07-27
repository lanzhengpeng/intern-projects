### 【2943】swap all components to empty reference set 把所有组件交换到空参考集

#### 代码

```cpp
    /*HEAD SWAP_ALL_COMPONENTS_TO_EMPTY_REFERENCE_SET CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：注意，UF_print_syslog是V18版本新增的，只回答翻译，不要废话。 */  
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
        swap_all_components_reference_set("Empty");  
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

> 这段代码是NX二次开发的一个UF示例程序，其主要功能是将工作部件中的所有组件移入名为"Empty"的参考集。
>
> 代码中的主要函数和步骤包括：
>
> 1. swap_all_components_reference_set(char *refset)：该函数获取工作部件的所有组件集合，并将这些组件移入指定的参考集。
> 2. do_it(void)：调用swap_all_components_reference_set函数，将所有组件移入名为"Empty"的参考集。
> 3. ufusr(char *param, int *retcode, int paramLen)：NX二次开发的标准入口函数。在该函数中，首先调用UF_initialize进行初始化，然后执行do_it函数，最后调用UF_terminate进行清理。
> 4. report_error(char *file, int line, char *call, int irc)：该函数用于在UF函数调用失败时，报告错误信息。
> 5. ufusr_ask_unload(void)：该函数返回UF_UNLOAD_IMMEDIATELY，表示该程序可以被立即卸载。
>
> 综上所述，该代码的主要功能是获取工作部件中的所有组件，并将这些组件移入名为"Empty"的参考集。它是一个典型的NX二次开发示例程序。
>
