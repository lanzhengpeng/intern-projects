### 【2334】report selected feat ug version 选择特征报告UG版本

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_FEAT_UG_VERSION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，关于 UF_print_syslog 的翻译如下：

UF_print_syslog 是在 V18 中新增的函数，用于打印系统日志。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
    }  
    static void do_it(void)  
    {  
        int  
            cre_version,  
            gmtime,  
            mod_version,  
            num,  
            version;  
        tag_t  
            feat,  
            part = UF_PART_ask_display_part();  
        char  
            *program,  
            *machine,  
            *user;  
        UF_PART_history_list_p_t  
            history_list = NULL;  
        UF_CALL(UF_PART_create_history_list(&history_list));  
        while ((feat = select_a_feature("Select feature to query")) != NULL_TAG)  
        {  
            UF_CALL(UF_PART_ask_part_history(part, history_list));  
            UF_CALL(UF_PART_ask_num_histories(history_list, &num));  
            UF_CALL(UF_OBJ_ask_cre_mod_versions(feat, &cre_version, &mod_version));  
            UF_CALL(UF_PART_ask_nth_history(history_list, num - cre_version,  
                         &program, &user, &machine, &version, &gmtime));  
            WRITE_D(cre_version);  
            WRITE_S(program);  
            UF_CALL(UF_PART_ask_nth_history(history_list, num - mod_version,  
                         &program, &user, &machine, &version, &gmtime));  
            WRITE_D(mod_version);  
            WRITE_S(program);  
            UF_CALL(UF_PART_clear_history_list(history_list ));  
        }  
        UF_CALL(UF_PART_delete_history_list(history_list));  
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

> 根据代码，这是一段NX Open C++ API的二次开发代码，主要功能是查询选中特征的历史信息。
>
> 主要逻辑如下：
>
> 1. 包含了NX Open C++ API的头文件。
> 2. 定义了错误报告函数report_error()，用于打印错误信息。
> 3. 定义了整数和字符串写入列表窗口的宏WRITE_D()和WRITE_S()。
> 4. 定义了选择特征的函数select_a_feature()，用于让用户选择一个特征。
> 5. 定义了执行主逻辑的函数do_it()，其流程是：获取当前显示的部件。创建部件历史列表。循环让用户选择特征，对每个特征执行：获取特征创建和修改的版本号。获取特征对应的创建和修改历史信息。将版本号和历史信息写入列表窗口。释放历史列表。
> 6. 获取当前显示的部件。
> 7. 创建部件历史列表。
> 8. 循环让用户选择特征，对每个特征执行：获取特征创建和修改的版本号。获取特征对应的创建和修改历史信息。将版本号和历史信息写入列表窗口。
> 9. 获取特征创建和修改的版本号。
> 10. 获取特征对应的创建和修改历史信息。
> 11. 将版本号和历史信息写入列表窗口。
> 12. 释放历史列表。
> 13. 定义了ufusr()函数，作为程序的入口，初始化NX Open环境，调用do_it()执行主逻辑，然后终止NX Open环境。
> 14. 定义了ufusr_ask_unload()函数，返回立即卸载标志。
>
> 总体来说，这段代码通过交互方式让用户选择特征，并查询显示每个特征的创建和修改历史信息。
>
