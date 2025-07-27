### 【2184】report line arrow preferences 报告线箭头首选项

#### 代码

```cpp
    /*HEAD REPORT_LINE_ARROW_PREFERENCES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = %d\n", title, n);  
        ECHO(msg);  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = %f\n", title, n);  
        ECHO(msg);  
    }  
    static void report_line_arrow_preferences(UF_DRF_line_arrow_preferences_t  
        *line_arrow_preferences)  
    {  
        WRITE_D(line_arrow_preferences->first_arrow_type);  
        WRITE_D(line_arrow_preferences->second_arrow_type);  
        WRITE_D(line_arrow_preferences->leader_location);  
        WRITE_F(line_arrow_preferences->arrowhead_length);  
        WRITE_F(line_arrow_preferences->arrowhead_included_angle);  
        WRITE_F(line_arrow_preferences->dot_arrowhead_diameter);  
        WRITE_F(line_arrow_preferences->stub_length);  
        WRITE_F(line_arrow_preferences->text_to_line_distance);  
        WRITE_F(line_arrow_preferences->line_past_arrow_distance);  
        WRITE_F(line_arrow_preferences->oblique_extension_line_angle);  
        WRITE_F(line_arrow_preferences->first_pos_to_extension_line_distance);  
        WRITE_F(line_arrow_preferences->second_pos_to_extension_line_distance);  
        WRITE_F(line_arrow_preferences->datum_length_past_arrow);  
        WRITE_F(line_arrow_preferences->text_over_stub_factor);  
        WRITE_D(line_arrow_preferences->first_extension_line_cfw.color);  
        WRITE_D(line_arrow_preferences->first_extension_line_cfw.font);  
        WRITE_D(line_arrow_preferences->first_extension_line_cfw.width);  
        WRITE_D(line_arrow_preferences->first_arrowhead_cfw.color);  
        WRITE_D(line_arrow_preferences->first_arrowhead_cfw.font);  
        WRITE_D(line_arrow_preferences->first_arrowhead_cfw.width);  
        WRITE_D(line_arrow_preferences->first_arrow_line_cfw.color);  
        WRITE_D(line_arrow_preferences->first_arrow_line_cfw.font);  
        WRITE_D(line_arrow_preferences->first_arrow_line_cfw.width);  
        WRITE_D(line_arrow_preferences->second_extension_line_cfw.color);  
        WRITE_D(line_arrow_preferences->second_extension_line_cfw.font);  
        WRITE_D(line_arrow_preferences->second_extension_line_cfw.width);  
        WRITE_D(line_arrow_preferences->second_arrowhead_cfw.color);  
        WRITE_D(line_arrow_preferences->second_arrowhead_cfw.font);  
        WRITE_D(line_arrow_preferences->second_arrowhead_cfw.width);  
        WRITE_D(line_arrow_preferences->second_arrow_line_cfw.color);  
        WRITE_D(line_arrow_preferences->second_arrow_line_cfw.font);  
        WRITE_D(line_arrow_preferences->second_arrow_line_cfw.width);  
        WRITE_F(line_arrow_preferences->arrow_out_length_factor);  
    }  
    static void do_it(void)  
    {  
        double  
            old_stub_length;  
        UF_DRF_line_arrow_preferences_t  
            line_arrow_preferences;  
        UF_CALL(UF_DRF_ask_line_arrow_preferences(&line_arrow_preferences));  
        report_line_arrow_preferences(&line_arrow_preferences);  
        old_stub_length = line_arrow_preferences.stub_length;  
        line_arrow_preferences.stub_length = 0;  
        UF_CALL(UF_DRF_set_line_arrow_preferences(&line_arrow_preferences));  
        UF_CALL(UF_DRF_ask_line_arrow_preferences(&line_arrow_preferences));  
        report_line_arrow_preferences(&line_arrow_preferences);  
        line_arrow_preferences.stub_length = old_stub_length;  
        UF_CALL(UF_DRF_set_line_arrow_preferences(&line_arrow_preferences));  
        UF_CALL(UF_DRF_ask_line_arrow_preferences(&line_arrow_preferences));  
        report_line_arrow_preferences(&line_arrow_preferences);  
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

> 这段代码是用于NX二次开发的C++代码，其主要功能是获取和设置NX中的直线箭头标注偏好设置。具体来说：
>
> 1. 定义了宏ECHO和UF_CALL，用于输出错误信息。
> 2. 定义了宏WRITE_D和WRITE_F，用于将整型和浮点型数据输出到列表窗口。
> 3. 定义了函数report_line_arrow_preferences，用于输出直线箭头标注的偏好设置。
> 4. 定义了函数do_it，用于获取当前的直线箭头标注设置，修改stub_length，然后再次获取设置。
> 5. 在ufusr函数中，首先初始化NX，然后调用do_it函数，最后终止NX。
> 6. ufusr_ask_unload函数用于立即卸载二次开发代码。
>
> 总的来说，这段代码主要用于测试NX中直线箭头标注的获取和设置函数，并通过打印到列表窗口来显示设置值。
>
