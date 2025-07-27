### 【2526】save current view 保存当前视图

#### 代码

```cpp
    /*HEAD SAVE_CURRENT_VIEW CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_view.h>  
    #include <uf_layout.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：UF_print_syslog是V18版本新增的函数。 */  
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
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #include <uf_undo.h>  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
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
        int  
            n;  
        tag_t  
            work_view;  
        char  
            old_view_name[MAX_ENTITY_NAME_SIZE+1] = { "" },  
            new_view_name[MAX_ENTITY_NAME_SIZE+1];  
        WRITENZ(uc6472("", &n));  
        if (n == 1)  
        {  
            WRITENZ(uc6473("", old_view_name));  
        }  
        else  
        {  
            UF_CALL(UF_VIEW_ask_work_view(&work_view));  
            UF_CALL(UF_OBJ_ask_name(work_view, old_view_name));  
        }  
        strcpy(new_view_name, old_view_name);  
        if (prompt_for_text("Enter new view name", new_view_name))  
        {  
            set_undo_mark("Save current view");  
            WRITENZ(uc6450(old_view_name, new_view_name, TRUE, FALSE));  
        }  
    }  
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

> 根据代码，这是一段NX的二次开发代码，主要实现以下功能：
>
> 1. 定义错误报告函数report_error，用于在调用NX API函数出错时打印错误信息。
> 2. 定义写整数到列表窗口的函数write_integer_to_listing_window，用于将整数写入NX的列表窗口。
> 3. 定义设置撤销标记的函数set_undo_mark，用于在撤销树中设置一个标记点。
> 4. 定义提示输入文本的函数prompt_for_text，用于弹出一个对话框提示用户输入文本。
> 5. 定义主函数do_it，用于获取当前工作视图的名称，提示用户输入新视图名称，并调用API保存当前视图为新视图。
> 6. 定义ufusr函数，作为NX的二次开发入口函数，初始化NX环境，调用主函数do_it，然后终止NX环境。
> 7. 定义ufusr_ask_unload函数，用于设置二次开发程序的卸载模式。
>
> 总的来说，这段代码实现了在NX中保存当前视图为新视图的功能，通过二次开发实现了交互式的视图管理。
>
