### 【1491】label selected drawing member view with its name 为选定的绘图成员视图添加其名称标签

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_draw.h>  
    #include <uf_view.h>  
    #include <stdarg.h>  
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
    static tag_t select_drawing_member_view(char *prompt)  
    {  
        int  
            orig_cv,  
            resp;  
        tag_t  
            view_tag;  
        double  
            pos[3];  
        UF_CALL(UF_UI_ask_cursor_view(&orig_cv));  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_specify_screen_position(prompt, NULL, NULL, pos,  
            &view_tag, &resp));  
        UF_CALL(UF_UI_set_cursor_view(orig_cv));  
        if (resp == UF_UI_PICK_RESPONSE)  
            return view_tag;  
        else  
            return NULL_TAG;  
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
        tag_t  
            view = NULL_TAG,  
            view_label = NULL_TAG;  
        UF_DRAW_view_label_parms_t  
            view_label_parms;  
        while ((view = select_drawing_member_view("Add view label")) != NULL_TAG)  
        {  
            UF_CALL(UF_DRAW_ask_view_label(view, &view_label));  
            view_label_parms.view_label_parm_type = UF_DRAW_view_label_other_view;  
            UF_CALL(UF_DRAW_ask_view_label_parms(view_label, &view_label_parms));  
        //  force the view label to be ON  
            view_label_parms.view_label = TRUE;  
        //  the label text will be the view's name  
            view_label_parms.view_label_type = UF_DRAW_view_label_view_name;  
            if (view_label == NULL_TAG)  
                UF_CALL(UF_DRAW_create_view_label(view, &view_label_parms,  
                    &view_label));  
            else  
                UF_CALL(UF_DRAW_set_view_label_parms(view_label,  
                    &view_label_parms));  
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

> 这是段NX二次开发代码，主要功能是在NX中添加视图标签。以下是代码的主要功能：
>
> 1. 定义了ECHO和report_error两个函数，用于打印信息和错误报告。
> 2. select_drawing_member_view函数用于让用户选择一个视图，并返回视图的tag。
> 3. prompt_for_text函数用于获取用户输入的文本。
> 4. do_it函数是主函数，循环让用户选择视图，然后为每个视图添加或修改视图标签，标签内容为视图名称，标签位置为视图中心。
> 5. ufusr函数是NX二次开发程序的入口函数，调用do_it函数。
> 6. ufusr_ask_unload函数用于卸载二次开发程序。
>
> 该代码的主要功能是给NX中的每个视图添加视图名称标签，以便于区分不同的视图。
>
