### 【1351】get view model view part 获取视图对应的模型视图部分

#### 代码

```cpp
    /*HEAD GET_VIEW_MODEL_VIEW_PART CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_draw.h>  
    #include <uf_view.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            view = NULL_TAG;  
        char    
            model_view_partname[MAX_FSPEC_SIZE + 1] = { "" };  
        while ((view = select_drawing_member_view("Member View:")) != NULL_TAG)  
        {  
            UF_CALL( UF_DRAW_get_view_model_view_part( view,   
                                         model_view_partname ));  
            WRITE_S( model_view_partname );  
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

> 这段代码是用于NX二次开发的UF程序，主要功能是获取成员视图的模型视图部件名称。具体介绍如下：
>
> 1. 定义了错误报告函数report_error，用于在函数调用出错时报告错误信息。
> 2. 定义了选择绘图成员视图函数select_drawing_member_view，用于提示用户选择一个视图，并返回视图的tag。
> 3. 定义了向列表窗口写字符串的函数write_string_to_listing_window，用于向列表窗口输出字符串信息。
> 4. 定义了主函数do_it，循环调用select_drawing_member_view获取视图tag，然后调用UF_DRAW_get_view_model_view_part获取模型视图部件名称，并输出。
> 5. 定义了ufusr函数，初始化后调用do_it，然后终止。
> 6. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 总的来说，该程序通过用户交互选择视图，获取视图对应的模型视图部件名称，并在列表窗口输出，实现了获取成员视图模型视图部件名称的功能。
>
