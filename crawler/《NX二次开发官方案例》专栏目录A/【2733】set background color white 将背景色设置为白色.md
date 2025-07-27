### 【2733】set background color white 将背景色设置为白色

#### 代码

```cpp
    /*HEAD SET_BACKGROUND_COLOR_WHITE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog在V18版本中是新增的，请只回答译文，不要添加其他无关内容。 */  
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
    /*  
        This function sets and loads the color table background color  
        red, green and blue values should be doubles between zero and 1.  
        1.0, 1.0, 1.0 = white, 0.0, 0.0, 0.0 = black, 0.0, 0.0, 1.0 = blue  
    */  
    void set_background_color(double redval, double grnval, double bluval)  
    {  
        double clr_values[3];  
        clr_values[0] = redval;  
        clr_values[1] = bluval;  
        clr_values[2] = grnval;  
        UF_CALL(UF_DISP_set_color(UF_DISP_BACKGROUND_COLOR,  
            UF_DISP_rgb_model, "", clr_values ));  
        UF_CALL(UF_DISP_load_color_table());  
    }  
    static void do_it(void)  
    {  
        set_background_color(1.0, 1.0, 1.0);  
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

> 这是用于设置NX背景颜色的二次开发代码。主要功能包括：
>
> 1. 定义错误报告函数report_error，用于输出函数调用失败信息。
> 2. 定义set_background_color函数，用于设置背景颜色。该函数接受红、绿、蓝三个颜色通道的值(0-1之间)，并调用UF_DISP_set_color设置颜色，再调用UF_DISP_load_color_table加载颜色表。
> 3. 定义do_it函数，用于将背景颜色设置为白色。
> 4. 定义ufusr函数，作为二次开发的入口函数。在函数内部，先调用UF_initialize初始化NX，然后调用do_it设置背景颜色，最后调用UF_terminate结束二次开发。
> 5. 定义ufusr_ask_unload函数，返回立即卸载二次开发代码。
>
> 通过这段代码，用户可以方便地设置NX的背景颜色。作者通过封装函数，使代码更加清晰易读，并且方便后续修改和维护。
>
