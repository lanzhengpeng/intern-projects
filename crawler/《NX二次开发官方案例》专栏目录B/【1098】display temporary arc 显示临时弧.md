### 【1098】display temporary arc 显示临时弧

#### 代码

```cpp
    /*HEAD DISPLAY_TEMPORARY_ARC CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_mtx.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中的新功能，请只提供翻译，不要添加任何废话。 */  
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
        double  
            abs_center[3] = { 1,2,3 },  
            arc_center[3],  
            yz[9] = { 0,1,0, 0,0,1, 1,0,0 };  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_MTX3_vec_multiply(abs_center, yz, arc_center);  
    /*  This code contains the work around for PR 664013 - Fixed in V15.0  
        double  
            arc_matrix[9];  
        UF_MTX3_transpose(yz, arc_matrix);  
        UF_CALL(UF_DISP_display_temporary_arc(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            arc_matrix, 0, PI, arc_center, 1.0, &attrib));  
    */  
        UF_CALL(UF_DISP_display_temporary_arc(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            yz, 0, PI, arc_center, 1.0, &attrib));  
        uc1601("Arc is in YZ plane at 1,2,3 ABS", TRUE);  
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

> 这是一段NX二次开发代码，主要用于在NX中绘制一个临时圆弧。以下是代码的主要功能和步骤：
>
> 1. 包含了NX提供的头文件，以调用NX的API函数。
> 2. 定义了一个错误报告函数report_error，用于在调用NX API函数失败时输出错误信息。
> 3. 定义了主要功能函数do_it，其中：
>
> 1. 定义了ufusr函数，这是NX二次开发程序的入口函数。在该函数中，先调用UF_initialize初始化，然后调用do_it函数绘制圆弧，最后调用UF_terminate结束。
> 2. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示二次开发程序被卸载时不需要等待。
> 3. 注释掉了一段旧代码，这是针对PR 664013问题的修复，在V15.0版本中已经修复。
>
> 综上所述，该代码的主要功能是在NX中绘制一个位于YZ平面，圆心在绝对坐标(1,2,3)，半径为1的白色临时圆弧。
>
