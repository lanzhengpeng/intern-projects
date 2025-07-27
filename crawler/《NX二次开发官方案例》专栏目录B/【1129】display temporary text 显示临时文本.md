### 【1129】display temporary text 显示临时文本

#### 代码

```cpp
    /*HEAD DISPLAY_TEMPORARY_TEXT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_assem.h>  
    #include <uf_disp.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    static void ECHO(char* format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN + 1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error(char* file, int line, char* call, int irc)  
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
    static void do_it(void)  
    {  
        char  
            msg[133];  
        int  
            units = UF_PART_METRIC,  
            color_index = 1;  
        double  
            textSize = 1.,  
            pt[3] = { 1.0, 1.0, 1.0 };  
        UF_OBJ_disp_props_t  
            color = { 1, color_index, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE };  
        sprintf(msg, "YOUR TEXT HERE");  
        UF_CALL(UF_DISP_ask_closest_color_in_part(UF_PART_ask_display_part(), UF_DISP_RED_NAME, &color_index));  
        color.color = color_index;  
        UF_CALL(UF_PART_ask_units(UF_PART_ask_display_part(), &units));  
        if (units == UF_PART_METRIC)  
            textSize = 12.;  
        else  
            textSize = .5;  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_ACTIVE_MINUS,  
            msg, pt, UF_DISP_MIDDLECENTER, &color, 10.0, 1));  
    }  
    void ufusr(char* param, int* retcode, int paramLen)  
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

> 这段代码是用于NX CAD软件的二次开发示例，主要实现了在当前活动部件上显示临时文字的功能。
>
> 主要功能包括：
>
> 1. 定义了ECHO函数，用于在日志窗口和系统日志中输出信息。
> 2. 定义了report_error函数，用于在函数调用失败时报告错误。
> 3. 定义了do_it函数，用于执行显示临时文字的主要逻辑：获取当前活动部件的颜色索引。获取当前活动部件的长度单位。根据长度单位设置文字大小。使用UF_DISP_display_temporary_text函数在指定位置显示指定颜色的文字。
> 4. 获取当前活动部件的颜色索引。
> 5. 获取当前活动部件的长度单位。
> 6. 根据长度单位设置文字大小。
> 7. 使用UF_DISP_display_temporary_text函数在指定位置显示指定颜色的文字。
> 8. 定义了ufusr函数，作为NX二次开发的入口函数，用于初始化和调用do_it函数。
> 9. 定义了ufusr_ask_unload函数，返回卸载类型为立即卸载。
>
> 总体来说，这段代码是一个典型的NX二次开发示例，实现了在当前活动部件上显示临时文字的功能，具有很好的参考价值。
>
