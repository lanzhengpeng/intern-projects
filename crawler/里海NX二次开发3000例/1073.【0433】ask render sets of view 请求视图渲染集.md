### 【0433】ask render sets of view 请求视图渲染集

#### 代码

```cpp
    /*HEAD ASK_RENDER_SETS_OF_VIEW CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_view.h>  
    #include <uf_draw.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：注意：UF_print_syslog在V18中是新的，只需回答翻译，不要添加无关内容。 */  
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
    static tag_t select_drawing_member_view(void)  
    {  
        tag_t  
            view_tag;  
        double  
            rr2[3];  
        char  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_UI_set_cursor_view(FALSE));  
        if (uc1615("Select member view", rr2) == 5)  
        {  
            uc1653(view_name);  
            if (!UF_CALL(UF_VIEW_ask_tag_of_view_name(view_name, &view_tag)))  
                return view_tag;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            *sets,  
            view;  
        int  
            n;  
        while ((view = select_drawing_member_view()) != NULL_TAG)  
        {  
            UF_CALL(UF_DRAW_ask_render_sets_of_view(view, &n, &sets));  
            ECHO(n);  
            if (n > 0) UF_free(sets);  
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

> 根据代码，这是用于NX的二次开发代码，主要功能是获取并打印指定视图的渲染集合信息。具体包括以下内容：
>
> 1. 包含了必要的NX Open头文件，用于访问NX的函数。
> 2. 定义了两个宏：ECHO用于打印变量的值，UF_CALL用于调用NX函数并报告错误。
> 3. 定义了一个报告错误的函数report_error，用于打印错误信息。
> 4. 定义了一个函数select_drawing_member_view，用于选择一个视图，并获取其视图标签。
> 5. 定义了一个函数do_it，用于循环选择视图，并获取每个视图的渲染集合数量和渲染集合标签数组，然后打印渲染集合数量。
> 6. 定义了ufusr函数，用于初始化NX，执行do_it函数，然后终止NX。
> 7. 定义了一个询问卸载函数，表示立即卸载。
> 8. ufusr是主函数，在NX启动时调用，用于执行主要功能。
> 9. 整个代码的主要逻辑是获取指定视图的渲染集合信息，并打印渲染集合数量。
>
