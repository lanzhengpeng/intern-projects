### 【1700】modify current drawings projection angle 修改当前图纸的投影角度

#### 代码

```cpp
    /*HEAD MODIFY_CURRENT_DRAWINGS_PROJECTION_ANGLE CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog 是 V18 新增的，请只回答翻译，不要添加其他内容。 */  
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
        tag_t  
            drawing;  
        UF_DRAW_info_t  
            drawing_info;  
        if (!UF_CALL(UF_DRAW_ask_current_drawing(&drawing)) &&  
            (drawing != NULL_TAG))  
        {  
            UF_CALL(UF_DRAW_ask_drawing_info(drawing, &drawing_info));  
            if (drawing_info.projection_angle == UF_DRAW_THIRD_ANGLE_PROJECTION)  
                drawing_info.projection_angle = UF_DRAW_FIRST_ANGLE_PROJECTION;  
            else  
                drawing_info.projection_angle = UF_DRAW_THIRD_ANGLE_PROJECTION;  
            UF_CALL(UF_DRAW_set_drawing_info(drawing, &drawing_info));  
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

> 这段代码是NX的二次开发代码，主要功能是修改当前绘图的投影角度。具体功能如下：
>
> 1. 定义了错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 定义了do_it函数，用于获取当前绘图对象，查询其投影角度信息，并切换到另一投影角度。
> 3. ufusr函数是NX二次开发的主入口函数。在这个函数中，首先调用UF_initialize函数进行初始化，然后调用do_it函数执行修改投影角度的操作，最后调用UF_terminate函数结束程序。
> 4. ufusr_ask_unload函数用于告诉NX在卸载该代码时立即卸载。
>
> 通过调用UF_DRAW_ask_current_drawing、UF_DRAW_ask_drawing_info、UF_DRAW_set_drawing_info等函数，实现了查询当前绘图对象、获取绘图信息、设置绘图信息等操作。另外，通过宏定义UF_CALL，简化了错误处理流程。
>
> 总体来说，这段代码结构清晰，实现了查询和修改当前绘图投影角度的功能。
>
