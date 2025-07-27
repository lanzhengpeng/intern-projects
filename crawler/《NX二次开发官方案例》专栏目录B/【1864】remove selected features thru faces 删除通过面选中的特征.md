### 【1864】remove selected features thru faces 删除通过面选中的特征

#### 代码

```cpp
    /*HEAD REMOVE_SELECTED_FEATURES_THRU_FACES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，以下是关键点的翻译：

1. UF_print_syslog是V18版本中新增的函数。
2. UF_print_syslog用于在系统日志中打印指定格式的消息。
3. UF_print_syslog的原型为：int UF_print_syslog(const char *fmt, ...);
4. fmt参数指定了要打印的消息格式，其他参数指定了要打印的实际数据。
5. UF_print_syslog返回值表示成功打印的消息长度。
6. UF_print_syslog可用于调试或记录系统运行过程中的重要信息。
7. UF_print_syslog的使用需要包含头文件<syslog.h>。
8. UF_print_syslog是线程安全的，可在多线程环境中使用。
9. UF_print_syslog可以自动在打印的消息前添加时间戳。
10. UF_print_syslog可以指定消息的日志级别，例如：警告、错误等。

UF_print_syslog是V18版本中新增的函数，用于在系统日志中打印指定格式的消息。该函数原型为：int UF_print_syslog(const char *fmt, ...)，其中fmt参数指定了要打印的消息格式，其他参数指定了要打印的实际数据。UF_print_syslog返回值表示成功打印的消息长度。该函数可用于调试或记录系统运行过程中的重要信息，并且需要包含头文件<syslog.h>。此外，UF_print_syslog是线程安全的，可以在多线程环境中使用，并且可以自动在打印的消息前添加时间戳，以及指定消息的日志级别。 */  
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
    #include <uf_undo.h>  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static logical ask_yes_or_no(char *title, char *message)  
    {  
        int  
            resp;  
        UF_UI_message_buttons_t  
            buttons = { TRUE, FALSE, TRUE, "Yes", NULL, "No", 1, 0, 2 };  
        UF_CALL(UF_UI_message_dialog(title, UF_UI_MESSAGE_QUESTION, &message, 1,  
            TRUE, &buttons, &resp));  
        return (2 - resp);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat;  
        while ((feat = select_a_feature("Remove thru faces")) != NULL_TAG)  
        {  
            set_undo_mark("Remove thru faces");  
            if (!UF_CALL(UF_MODL_remove_thru_faces(feat)))  
            {  
                if (!UF_CALL(UF_MODL_update()) &&  
                    ask_yes_or_no("PR 5897634", "Try work around?"))  
                    UF_CALL(UF_MODL_update_all_features());  
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

> 这是NX Open C++的一个二次开发示例代码，其主要功能是移除通过选择面而选择的所有特征。以下是该代码的主要功能介绍：
>
> 1. 引入了NX Open C++常用的头文件，如uf.h、uf_ui.h等。
> 2. 定义了report_error函数，用于在出错时报告错误信息，包括错误码、错误位置、错误函数等。
> 3. 定义了select_a_feature函数，用于提示用户选择一个特征，并返回特征ID。
> 4. 定义了set_undo_mark函数，用于设置撤销标记。
> 5. 定义了ask_yes_or_no函数，用于弹出一个Yes/No对话框。
> 6. 定义了do_it函数，是主逻辑函数：循环提示用户选择特征设置撤销标记调用UF_MODL_remove_thru_faces函数移除特征如果移除失败，尝试更新模型并提示是否绕过错误继续
> 7. 循环提示用户选择特征
> 8. 设置撤销标记
> 9. 调用UF_MODL_remove_thru_faces函数移除特征
> 10. 如果移除失败，尝试更新模型并提示是否绕过错误继续
> 11. 定义了ufusr函数，这是NX二次开发的入口函数，初始化后调用do_it函数，然后终止。
> 12. 定义了ufusr_ask_unload函数，用于卸载时立即删除所有撤销标记。
>
> 总的来说，这是一个通过用户选择面来移除特征的NX二次开发示例，涵盖了错误处理、用户交互、撤销与重做等功能。
>
