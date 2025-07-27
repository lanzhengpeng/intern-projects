### 【1512】list all views on current drawing 列出当前图纸的所有视图

#### 代码

```cpp
    /*HEAD LIST_ALL_VIEWS_ON_CURRENT_DRAWING CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_draw.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的函数。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
        int  
            ii,  
            n_views;  
        tag_t  
            dwg,  
            *views;  
        char  
            member_view[MAX_ENTITY_NAME_SIZE+1];  
        if (!UF_CALL(UF_DRAW_ask_current_drawing(&dwg)) && (dwg != NULL_TAG))  
        {  
            UF_CALL(UF_DRAW_ask_views(dwg, &n_views, &views));  
            WRITE_D(n_views);  
            for (ii = 0; ii < n_views; ii++)  
            {  
                UF_CALL(UF_OBJ_ask_name(views[ii], member_view));  
                WRITE_S(member_view);  
            }  
            if (n_views > 0) UF_free(views);  
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

> 这段NX二次开发代码主要用于列出当前绘图中的所有视图名称。以下是代码的主要功能和流程：
>
> 1. 定义了错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 定义了WRITE宏，用于将字符串写入列表窗口。
> 3. 定义了WRITE_D和WRITE_S宏，分别用于将整型和字符串变量及其值写入列表窗口。
> 4. do_it函数是代码的主要逻辑，包括以下步骤：获取当前绘图对象。获取绘图中的视图列表及视图数量。遍历视图列表，获取每个视图的名称，并输出到列表窗口。如果生成了视图列表，则释放内存。
> 5. 获取当前绘图对象。
> 6. 获取绘图中的视图列表及视图数量。
> 7. 遍历视图列表，获取每个视图的名称，并输出到列表窗口。
> 8. 如果生成了视图列表，则释放内存。
> 9. ufusr函数是NX二次开发程序的入口函数，在这里调用do_it函数执行主要逻辑。
> 10. ufusr_ask_unload函数用于设置程序卸载模式，这里设置为立即卸载。
>
> 整体而言，这段代码实现了列出当前绘图中的所有视图名称的功能，并包含了错误处理、内存管理等辅助功能。
>
