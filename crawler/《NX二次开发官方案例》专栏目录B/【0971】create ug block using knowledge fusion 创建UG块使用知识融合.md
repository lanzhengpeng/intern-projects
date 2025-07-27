### 【0971】create ug block using knowledge fusion 创建UG块使用知识融合

#### 代码

```cpp
    /*HEAD CREATE_UG_BLOCK_USING_KNOWLEDGE_FUSION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_kf.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能。

翻译如下：

注意：UF_print_syslog是V18版本新增的功能。 */  
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
            part = UF_PART_ask_display_part();  
        char  
            *generated_name,  
            *new_parameter_names[1] = { "Origin" },  
            *new_parameter_rules[1] = { "Point(3,0,0)" },  
            *old_parameter_names[3] = { "Length", "Width", "Height" },  
            *old_parameter_rules[3] = { "1.0", "2.0", "3.0" };  
        UF_CALL(UF_KF_init_part(part));  
        UF_CALL(UF_KF_create_child_rule(part, "root:", "cube", "ug_block",  
            3, old_parameter_names, old_parameter_rules, &generated_name));  
        UF_CALL(UF_MODL_update());  
        UF_CALL(UF_VIEW_fit_view(NULL_TAG, 1.0));  
        uc1601("Before modifying parameters", TRUE);  
        UF_CALL(UF_KF_create_child_rule(part, "root:", "cube", "ug_block",  
            1, new_parameter_names, new_parameter_rules, &generated_name));  
        UF_CALL(UF_MODL_update());  
        UF_CALL(UF_VIEW_fit_view(NULL_TAG, 1.0));  
        if (ask_yes_or_no("After modifying parameters", "Delete the rule?"))  
        {  
            UF_CALL(UF_KF_delete_instance_rule("root:", "cube"));  
            UF_CALL(UF_MODL_update());  
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

> 这段代码是一个NX Open C++ API的示例，用于演示如何使用知识融合(Knowledge Fusion)来创建和修改NX部件的参数化规则。
>
> 主要功能包括：
>
> 1. 初始化：使用UF_KF_init_part来初始化知识融合环境。
> 2. 创建规则：使用UF_KF_create_child_rule来创建一个cube的实例规则，并指定其尺寸参数。
> 3. 更新模型：调用UF_MODL_update来更新模型以反映规则更改。
> 4. 调整视图：调用UF_VIEW_fit_view来调整视图以适应模型。
> 5. 修改规则：再次调用UF_KF_create_child_rule来修改cube的实例规则，并更改其原点参数。
> 6. 确认删除：通过对话框询问用户是否删除规则，若确认则调用UF_KF_delete_instance_rule删除规则。
> 7. 错误处理：使用UF_CALL宏来捕获函数调用错误，并打印错误信息。
> 8. 初始化/终止：在ufusr函数中初始化和终止NX Open。
> 9. 卸载询问：定义了ufusr_ask_unload函数，用于询问立即卸载用户函数。
>
> 整体而言，这段代码提供了一个使用知识融合创建和修改参数化规则的示例，可供NX Open C++ API的二次开发人员参考。
>
