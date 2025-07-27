### 【2035】report configuration rules 报告配置规则

#### 代码

```cpp
    /*HEAD REPORT_CONFIGURATION_RULES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_ugmgr.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的信息，UF_print_syslog 是一个在 V18 版本中新增的函数。它主要用于打印系统日志信息。 */  
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
            active;  
        int  
            ii,  
            count;  
        char  
            msg[MAX_LINE_SIZE+1],  
            current_rule[UF_UGMGR_NAME_SIZE+1],  
            **config_rules;  
        UF_CALL(UF_is_ugmanager_active(&active));  
        if (!active)  
        {  
            UF_CALL(UF_UI_open_listing_window());  
            UF_CALL(UF_UI_write_listing_window("this is a not a UGMGR session\n"));  
            return;  
        }  
        UF_CALL(UF_UGMGR_ask_config_rule(current_rule));  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "Current Configuration Rule: %s\n",current_rule);  
        UF_CALL(UF_UI_write_listing_window(msg));  
        UF_CALL(UF_UGMGR_list_config_rules(&count, &config_rules));  
        UF_CALL(UF_UI_write_listing_window("\nAvailable Configuration Rules:\n"));  
        for (ii = 0; ii < count; ii++)  
        {  
            sprintf(msg, "\t%s\n", config_rules[ii]);  
            UF_CALL(UF_UI_write_listing_window(msg));  
        }  
        UF_CALL(UF_UI_write_listing_window("\n"));  
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
    int main(void)  
    {  
         const char  
             *args[1] = { "-a" };  
         if (UF_CALL(UF_UGMGR_initialize(1, args))) exit (1);  
         do_it();  
         UF_CALL(UF_terminate());  
         return 0;  
    }

```

#### 代码解析

> 这段代码是NX Open的二次开发代码，主要功能是查询和显示NX的配置规则。以下是代码的主要功能：
>
> 1. 包含必要的NX Open头文件，如uf.h, uf_ui.h等。
> 2. 定义了一个错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 3. 定义了一个主函数do_it，该函数完成以下功能：检查UGMGR是否激活，如果未激活则提示并返回。获取当前的配置规则并显示。列出所有可用的配置规则并显示。
> 4. 检查UGMGR是否激活，如果未激活则提示并返回。
> 5. 获取当前的配置规则并显示。
> 6. 列出所有可用的配置规则并显示。
> 7. 定义了ufusr函数，这是NX二次开发的入口函数。在这个函数中，初始化NX Open环境，调用do_it函数执行主要功能，然后终止NX Open环境。
> 8. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示二次开发模块可以立即卸载。
> 9. main函数是程序的入口，初始化UGMGR，调用do_it函数，然后终止UGMGR。
>
> 总的来说，这段代码通过NX Open API实现了查询和显示NX配置规则的功能，并且包含了错误处理和模块卸载的相关逻辑。
>
