### 【2810】set view border display off and on 设置视图边框显示开关

#### 代码

```cpp
    /*HEAD SET_VIEW_BORDER_DISPLAY_OFF_AND_ON CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的函数。 */  
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
    static void do_it(void)  
    {  
        UF_DRAW_set_border_display( FALSE );   
        uc1601("Border display is OFF", TRUE);  
        UF_DRAW_set_border_display( TRUE );   
        uc1601("Border display is On", TRUE);  
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

> 这是NX二次开发的UFUN函数，主要功能是设置和关闭NX的视图边框显示。以下是代码的主要功能：
>
> 1. 包含了UF开发所需的基本头文件。
> 2. 定义了一个错误报告函数report_error，用于在UF函数调用失败时打印错误信息。
> 3. 定义了do_it函数，用于执行设置边框显示的流程：调用UF_DRAW_set_border_display(false)来关闭边框显示。打印提示信息"Border display is OFF"。调用UF_DRAW_set_border_display(true)来开启边框显示。打印提示信息"Border display is On"。
> 4. 调用UF_DRAW_set_border_display(false)来关闭边框显示。
> 5. 打印提示信息"Border display is OFF"。
> 6. 调用UF_DRAW_set_border_display(true)来开启边框显示。
> 7. 打印提示信息"Border display is On"。
> 8. 定义了ufusr函数，是UFUN的入口函数：初始化UF系统。调用do_it函数执行设置边框显示流程。终止UF系统。
> 9. 初始化UF系统。
> 10. 调用do_it函数执行设置边框显示流程。
> 11. 终止UF系统。
> 12. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示在UFUN卸载时无需等待。
>
> 综上，该代码实现了通过UF函数设置和关闭NX视图边框显示的功能，并且在UF函数调用失败时可以打印错误信息。
>
