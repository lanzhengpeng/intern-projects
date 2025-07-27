### 【0728】create feature group 创建特征组

#### 代码

```cpp
    /*  
        Starting in NX7.5, the "hide_state" controls the "Embed Feature  
        Group Members" toggle on the Format-> Group-> Feature Group...  
        dialog.  This puts the memebers into a collapsible folder which is  
        the feature group in the Part Navigator.  In previous NX versions  
        it controls the "Hide Feature Set Members" toggle on the Format->  
        Group Features dialog and the members are not seen in the Part  
        Navigator at all.  
    */  
    #include <stdarg.h>  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
    #include <uf_ui.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    static int select_features(char *prompt, tag_t **feats)  
    {  
        int  
            cnt,  
            resp;  
        UF_UI_feat_sel_type_t  
            all = UF_UI_FEAT_SEL_TYPE_BROWSEABLE;  
        UF_CALL(UF_UI_select_feature(prompt, &all, &cnt, feats, &resp));  
        if (resp == UF_UI_OK) return cnt;  
        else return 0;  
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
    #include <uf_undo.h>  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void do_it(void)  
    {  
        logical  
            hide_state;  
        int  
            n;  
        tag_t  
            *feats,  
            grp;  
        if ((n = select_features("Feature Group", &feats)) == 0) return;  
        hide_state = ask_yes_or_no("Feature Group", "Embed Feature Group Members?");  
        set_undo_mark("Create Feature Group");  
        UF_CALL(UF_MODL_create_set_of_feature("GTAC", feats, n, hide_state, &grp));  
    }  
    extern void ufusr(char *param, int *retcode, int paramLen)  
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

> 这是一段NX二次开发代码，主要实现了以下功能：
>
> 1. 错误报告机制：使用宏UF_CALL来调用NX API，并在出错时报告错误信息，包括错误码、错误描述和出错位置。
> 2. 特征选择：定义了一个select_features函数，用于选择特征，并返回特征数量和特征数组。
> 3. 对话框询问：定义了一个ask_yes_or_no函数，用于弹出一个对话框询问用户Yes或No，并返回用户的选择结果。
> 4. 设置撤销标记：定义了一个set_undo_mark函数，用于在撤销堆栈中设置一个标记，以便在出错时可以撤销到这个标记处。
> 5. 创建特征组：定义了一个do_it函数，用于执行创建特征组的流程。首先选择特征，然后询问是否嵌入特征组，最后创建特征组并设置隐藏状态。
> 6. 主函数：定义了ufusr函数作为主函数，用于初始化NX环境，调用do_it函数执行创建特征组的流程，然后终止NX环境。
> 7. 卸载请求：定义了ufusr_ask_unload函数，用于在用户卸载NX时请求立即卸载。
>
> 整体来看，这段代码实现了在NX中创建特征组，并可以选择是否隐藏特征组内特征的功能。通过对话框交互、错误处理和撤销机制，提高了代码的健壮性和可用性。
>
