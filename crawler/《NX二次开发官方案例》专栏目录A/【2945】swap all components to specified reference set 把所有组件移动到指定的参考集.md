### 【2945】swap all components to specified reference set 把所有组件移动到指定的参考集

#### 代码

```cpp
    /*HEAD SWAP_ALL_COMPONENTS_TO_SPECIFIED_REFERENCE_SET CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的信息，UF_print_syslog是V18版本新增的功能。 */  
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
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
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
        char  
            refset[MAX_ENTITY_NAME_SIZE+1];  
        while (prompt_for_text(  
            "Reference Set name (Eg. None Empty SOLID, case sensitive)", refset))  
            swap_all_components_reference_set(refset);  
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

> 根据提供的NX二次开发代码，以下是对代码的介绍：
>
> 1. 代码功能：该代码实现了一个UF功能，可以将当前工作部件中的所有组件移动到指定的参考集中。
> 2. 主要函数：swap_all_components_reference_set(char *refset): 实现了移动组件到指定参考集的功能。do_it(): 循环提示用户输入参考集名称，并调用swap_all_components_reference_set函数实现移动组件。ufusr(char *param, int *retcode, int paramLen): NX二次开发标准入口函数，初始化后调用do_it()函数，然后终止。ufusr_ask_unload(): 指定卸载方式为立即卸载。
> 3. swap_all_components_reference_set(char *refset): 实现了移动组件到指定参考集的功能。
> 4. do_it(): 循环提示用户输入参考集名称，并调用swap_all_components_reference_set函数实现移动组件。
> 5. ufusr(char *param, int *retcode, int paramLen): NX二次开发标准入口函数，初始化后调用do_it()函数，然后终止。
> 6. ufusr_ask_unload(): 指定卸载方式为立即卸载。
> 7. 辅助函数：report_error(): 错误报告函数，用于打印UF函数调用失败的错误信息。prompt_for_text(): 提示用户输入文本信息。
> 8. report_error(): 错误报告函数，用于打印UF函数调用失败的错误信息。
> 9. prompt_for_text(): 提示用户输入文本信息。
> 10. 注意事项：使用宏UF_CALL简化了UF函数调用的错误处理。调用UF_print_syslog函数输出错误信息到系统日志。参考集名称需要区分大小写。
> 11. 使用宏UF_CALL简化了UF函数调用的错误处理。
> 12. 调用UF_print_syslog函数输出错误信息到系统日志。
> 13. 参考集名称需要区分大小写。
> 14. 二次开发环境：适用于NX 18及以上版本，使用了NX 18新增的UF_print_syslog函数。
> 15. 代码结构：遵循NX二次开发的一般规范，包括UF函数调用、错误处理、用户交互等。代码结构清晰，易于理解。
>
> 希望以上介绍能帮助您更好地理解这段NX二次开发代码。如果还有其他问题，欢迎继续提问。
>
