### 【2726】set all section views background off 设置所有剖面视图背景为关闭

#### 代码

```cpp
    /*HEAD SET_ALL_SECTION_VIEWS_BACKGROUND_OFF CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中的新功能。

因此，翻译为：UF_print_syslog是V18版本中的新功能，请只回答相关内容，不要添加无关的评论。 */  
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
        logical  
            is_section;  
        int  
            ii,  
            jj,  
            n_dwgs,  
            n_views;  
        tag_t  
            *dwgs,  
            *views;  
        char  
            view_name[MAX_ENTITY_NAME_SIZE + 1];  
        UF_DRAW_sxview_prfs_t  
            prfs;  
        UF_CALL(UF_DRAW_ask_drawings(&n_dwgs, &dwgs));  
        for (ii = 0; ii < n_dwgs; ii++)  
        {  
            UF_CALL(UF_DRAW_ask_views(dwgs[ii], &n_views, &views));  
            for (jj = 0; jj < n_views; jj++)  
            {  
                UF_CALL(UF_DRAW_is_sxview(views[jj], &is_section));  
                if (!is_section) continue;  
                UF_CALL(UF_DRAW_ask_sxview_display(views[jj], &prfs));  
                if (prfs.sx_background == UF_DRAW_sx_background_off) continue;  
                UF_CALL(UF_OBJ_ask_name(views[jj], view_name));  
                WRITE("Turning section background off in ");  
                WRITE_S(view_name);  
                prfs.sx_background = UF_DRAW_sx_background_off;  
                UF_CALL(UF_DRAW_set_sxview_display(views[jj], &prfs));  
                UF_CALL(UF_DRAW_update_one_view(dwgs[ii], views[jj]));  
            }  
            if (n_views > 0) UF_free(views);  
        }  
        if (n_dwgs > 0) UF_free(dwgs);  
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

> 这段代码是NX二次开发的一个UF程序，主要功能是遍历当前打开的NX图纸中的所有截面视图，并关闭它们的背景显示。
>
> 具体流程如下：
>
> 1. 首先调用UF_DRAW_ask_drawings函数获取当前打开的所有图纸。
> 2. 遍历每个图纸，调用UF_DRAW_ask_views函数获取图纸中的所有视图。
> 3. 遍历每个视图，首先判断它是否是截面视图，如果不是则跳过。
> 4. 如果是截面视图，则调用UF_DRAW_is_sxview函数判断它的背景显示是否已关闭，如果已关闭则跳过。
> 5. 如果背景显示未关闭，则调用UF_DRAW_set_sxview_display函数将其关闭，并更新视图。
> 6. 最后释放获取的视图和图纸数组。
>
> 代码中定义了一个错误报告函数report_error，用于打印错误信息。还定义了一个用于在列表窗口打印字符串的宏WRITE和WRITE_S。
>
> 主函数ufusr中初始化NX环境，调用do_it函数执行主要功能，最后终止NX环境。
>
> ufusr_ask_unload函数返回立即卸载，表示程序不需要卸载过程。
>
> 这段代码通过遍历所有视图并关闭它们的背景显示，实现了关闭所有截面视图背景的功能。
>
