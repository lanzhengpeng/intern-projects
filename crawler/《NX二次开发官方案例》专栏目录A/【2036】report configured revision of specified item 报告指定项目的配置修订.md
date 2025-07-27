### 【2036】report configured revision of specified item 报告指定项目的配置修订

#### 代码

```cpp
    /*HEAD REPORT_CONFIGURED_REVISION_OF_SPECIFIED_ITEM CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog是V18版本中新增的，请只回答翻译内容，无需添加其他内容。 */  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static logical ask_configured_rev(char *part_number, char *revision_id)  
    {  
        UF_UGMGR_tag_t  
            database_part_tag,  
            part_revision;  
        strcpy(revision_id, "");  
        if (UF_CALL(UF_UGMGR_ask_part_tag(part_number, &database_part_tag)) ||  
            UF_CALL(UF_UGMGR_ask_configured_rev(database_part_tag,  
                &part_revision)) ||  
            UF_CALL(UF_UGMGR_ask_part_revision_id(part_revision, revision_id)))  
            return FALSE;  
        return TRUE;  
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
    static void do_it(void)  
    {  
        logical  
            is_active;  
        char  
            current_rule[UF_UGMGR_NAME_SIZE + 1],  
            part_number[UF_UGMGR_NAME_SIZE + 1] = { "" },  
            revision_id[UF_UGMGR_NAME_SIZE + 1] = { "" },  
            use_rule[UF_UGMGR_NAME_SIZE + 1];  
        if (UF_CALL(UF_is_ugmanager_active(&is_active)) || !is_active) return;  
        UF_CALL(UF_UGMGR_ask_config_rule(current_rule));  
        WRITE_S(current_rule);  
        while ( strcmp(current_rule, use_rule) &&  
                strcpy(use_rule, current_rule) &&  
                prompt_for_text("Rule to use", use_rule) &&  
                strcmp(current_rule, use_rule))  
        {  
            UF_CALL(UF_UGMGR_set_config_rule(use_rule));  
            UF_CALL(UF_UGMGR_ask_config_rule(current_rule));  
            WRITE_S(current_rule);  
        }  
        while (prompt_for_text("Item ID to check", part_number))  
        {  
            if (ask_configured_rev(part_number, revision_id))  
            {  
                WRITE_S(part_number);  
                WRITE_S(revision_id);  
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

> 这段代码是NX的二次开发代码，主要功能是查询和设置NX配置管理器的配置规则，并查询指定部件的配置版本。代码的关键功能如下：
>
> 1. 定义了错误报告函数report_error，用于打印错误信息到系统日志和列表窗口。
> 2. 定义了字符串写入函数write_string_to_listing_window，用于将字符串写入列表窗口。
> 3. 定义了查询配置版本的函数ask_configured_rev，通过部件编号查询其在配置管理器中的配置版本。
> 4. 定义了提示输入文本的函数prompt_for_text，用于提示用户输入文本。
> 5. 在do_it函数中，首先检查配置管理器是否激活，然后获取和设置配置规则，并循环提示用户输入部件编号，查询其配置版本并打印。
> 6. ufusr函数是NX二次开发的入口函数，初始化后调用do_it函数，然后终止。
> 7. ufusr_ask_unload函数用于立即卸载二次开发应用程序。
>
> 总体来说，这段代码实现了与NX配置管理器的交互，让用户可以查询和设置配置规则，以及查询指定部件的配置版本，从而实现了配置管理功能。
>
