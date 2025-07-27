### 【2358】report selected objects ug version 报告选定的对象UG版本

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_OBJECTS_UG_VERSION CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本新增的函数。 */  
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
    static tag_t select_an_object(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an object", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, NULL,NULL,&resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
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
            obj,  
            part = UF_PART_ask_display_part();  
        char  
            *program,  
            *machine,  
            *user;  
        UF_PART_history_list_p_t  
            history_list = NULL;  
        UF_CALL(UF_PART_create_history_list(&history_list));  
        while ((obj = select_an_object("Select object to query")) != NULL_TAG)  
        {  
            UF_CALL(UF_PART_ask_part_history(part, history_list));  
            UF_CALL(UF_PART_ask_num_histories(history_list, &num));  
            UF_CALL(UF_OBJ_ask_cre_mod_versions(obj, &cre_version, &mod_version));  
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

> 根据代码的内容，这是用于NX 9.0及以下版本的二次开发代码，主要实现了以下功能：
>
> 1. 定义了错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 定义了整数写入列表窗口的函数write_integer_to_listing_window和字符串写入列表窗口的函数write_string_to_listing_window。
> 3. 定义了选择对象的函数select_an_object，用于通过对话框让用户选择一个对象。
> 4. 定义了主要函数do_it，用于实现以下功能：获取当前显示的零件创建零件历史记录列表循环让用户选择对象，对于每个选中的对象，查询其创建版本号、修改版本号以及对应的历史记录信息（创建和修改的版本号对应的历史记录），并将信息写入列表窗口。清空历史记录列表删除历史记录列表
> 5. 获取当前显示的零件
> 6. 创建零件历史记录列表
> 7. 循环让用户选择对象，对于每个选中的对象，查询其创建版本号、修改版本号以及对应的历史记录信息（创建和修改的版本号对应的历史记录），并将信息写入列表窗口。
> 8. 清空历史记录列表
> 9. 删除历史记录列表
> 10. 定义了ufusr函数，用于初始化NX，调用do_it函数执行主要功能，并在结束时终止NX。
> 11. 定义了卸载函数ufusr_ask_unload，用于立即卸载用户函数。
>
> 总体来说，这是一个用于查询零件对象历史记录信息的NX二次开发代码。
>
