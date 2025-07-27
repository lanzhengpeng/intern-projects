### 【2797】set text angle and font 设置文本角度和字体

#### 代码

```cpp
    /*HEAD SET_TEXT_ANGLE_AND_FONT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
    #include <uf_ugfont.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog在V18中新增。

仅提供翻译，避免不必要的废话。 */  
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
    static tag_t ask_font_table_entity(void)  
    {  
        tag_t  
            fte = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_font_table_type, &fte));  
        return (fte);  
    }  
    static int ask_font_index_number(char *font_name)  
    {  
        int  
            index;  
        tag_t  
            fte = ask_font_table_entity();  
        UF_CALL(UF_UGFONT_add_font(fte, &index, font_name));  
        return (index);  
    }  
    static void do_it(void)  
    {  
        UF_DRF_lettering_preferences_t  
            lettering_preferences;  
        UF_CALL(UF_DRF_ask_lettering_preferences(&lettering_preferences));  
        lettering_preferences.angle = 0;  
        lettering_preferences.general_text.cfw.font =  
            ask_font_index_number("leroy");  
        UF_CALL(UF_DRF_set_lettering_preferences(&lettering_preferences));  
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

> 这段代码是一个NX的二次开发示例，主要功能是设置NX的文本注释角度和字体。
>
> 代码主要包含以下几个部分：
>
> 1. 错误处理函数：定义了一个错误处理函数report_error，用于输出错误信息，以便调试。
> 2. 获取字体表实体：定义了一个函数ask_font_table_entity，用于获取当前显示部件的字体表实体。
> 3. 获取字体索引号：定义了一个函数ask_font_index_number，用于根据字体名称获取字体在字体表中的索引号。
> 4. 设置文本注释参数：定义了一个函数do_it，用于获取当前的文本注释参数，设置文本注释的角度为0，并设置文本注释的字体为“leroy”，然后保存设置。
> 5. ufusr函数：这是NX二次开发的入口函数，在这里调用do_it函数，执行设置文本注释参数的操作。
> 6. 卸载函数：定义了一个卸载函数ufusr_ask_unload，用于设置立即卸载二次开发程序。
>
> 通过这段代码，我们可以学习到如何使用NX的二次开发接口来设置文本注释的参数，包括注释角度和字体。这对于需要在NX中添加注释的二次开发应用非常有用。
>
