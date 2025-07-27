### 【1049】demo blank and reverse blank 演示隐藏和取消隐藏

#### 代码

```cpp
    /*HEAD DEMO_BLANK_AND_REVERSE_BLANK CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    static int select_objects(char *prompt, tag_t **objects)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_class_dialog("Select Objects", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, NULL, NULL, &resp, &cnt, objects));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*objects)[ii], 0));  
        return cnt;  
    }  
    static void set_objects_blank_status(int n, tag_t *objects, int status)  
    {  
        int  
            ii;  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_OBJ_set_blank_status(objects[ii], status));  
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
        int  
            n_objects;  
        tag_t  
            *objects;  
        set_undo_mark("Demo Blank/Reverse Blank All");  
        while ((n_objects = select_objects("Blank", &objects)) > 0)  
        {  
            set_objects_blank_status(n_objects, objects, UF_OBJ_BLANKED);  
            while (ask_yes_or_no("Reverse Blank All", "Reverse Blank All?"))  
                UF_CALL(UF_OBJ_reverse_blank_all());  
            set_objects_blank_status(n_objects, objects, UF_OBJ_NOT_BLANKED);  
            UF_free(objects);  
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

> 这段代码是一个NX Open C++应用程序，实现了以下功能：
>
> 1. 初始化和终止NX Open。
> 2. 定义了一个错误报告函数report_error，用于在发生错误时输出错误信息。
> 3. 定义了一个选择对象的函数select_objects，用于通过对话框选择对象，并高亮显示。
> 4. 定义了一个设置对象空白状态的函数set_objects_blank_status，用于将对象设置为空白或非空白。
> 5. 定义了一个询问是否的函数ask_yes_or_no，用于显示一个询问对话框，返回用户的选择。
> 6. 定义了一个设置撤销标记的函数set_undo_mark，用于设置一个撤销标记点。
> 7. 定义了一个主函数do_it，其中包含以下逻辑：设置撤销标记点循环选择对象，设置选中的对象为空白循环询问是否反转所有对象的空白状态，如果是则调用UF_OBJ_reverse_blank_all反转所有对象的空白状态将选中的对象设置为非空白释放对象数组
> 8. 设置撤销标记点
> 9. 循环选择对象，设置选中的对象为空白
> 10. 循环询问是否反转所有对象的空白状态，如果是则调用UF_OBJ_reverse_blank_all反转所有对象的空白状态
> 11. 将选中的对象设置为非空白
> 12. 释放对象数组
> 13. 定义了ufusr函数，作为程序的入口，调用do_it函数。
> 14. 定义了ufusr_ask_unload函数，用于卸载应用程序，返回UF_UNLOAD_IMMEDIATELY表示立即卸载。
>
> 总体来说，这段代码实现了一个简单的NX Open应用程序，用于设置和反转选定对象的空白状态，并具有撤销功能。
>
