### 【2754】set drafting aid text character font preference 设置绘图辅助文字字符字体偏好

#### 代码

```cpp
    /*HEAD SET_DRAFTING_AID_TEXT_CHARACTER_FONT_PREFERENCE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_ugfont.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中的新功能，用于打印系统日志。 */  
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
        int  
            mpi[100];  
        double  
            mpr[70];  
        char  
            rad[27],  
            dia[27];  
        UF_DRF_lettering_preferences_t  
            lettering_preferences;  
    /*  The old way - e.g. set font for General text 里海译:旧方法 - 例如为普通文本设置字体 */  
        UF_CALL(UF_DRF_ask_preferences(mpi, mpr, rad, dia));  
        mpi[88] = ask_font_index_number("leroy");  
        UF_CALL(UF_DRF_set_preferences(mpi, mpr, rad, dia));  
    /*  The new way - e.g. set font for Dimension text 里海译:新的方法 - 例如：为尺寸文本设置字体 */  
        UF_CALL(UF_DRF_ask_lettering_preferences(&lettering_preferences));  
        lettering_preferences.dimension_text.cfw.font =  
            ask_font_index_number("aurora");  
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

> 这段代码是NX的二次开发代码，主要功能是设置NX草图的文本字体偏好。
>
> 代码的主要流程如下：
>
> 1. 包含必要的头文件，如uf.h、uf_ui.h等。
> 2. 定义了report_error函数，用于打印错误信息。
> 3. 定义了ask_font_table_entity函数，用于获取字体表实体。
> 4. 定义了ask_font_index_number函数，用于查询字体名称对应的索引号。
> 5. 定义了do_it函数，用于设置字体偏好：获取当前的文本参数，如字体索引号。设置普通文本的字体索引号。获取当前的文本样式参数。设置尺寸文本的字体索引号。
> 6. 获取当前的文本参数，如字体索引号。
> 7. 设置普通文本的字体索引号。
> 8. 获取当前的文本样式参数。
> 9. 设置尺寸文本的字体索引号。
> 10. 定义了ufusr函数，是NX二次开发的入口函数，在NX启动时调用，执行do_it函数。
> 11. 定义了ufusr_ask_unload函数，用于卸载用户自定义函数。
>
> 通过调用ask_font_index_number函数，根据字体名称获取索引号，然后设置普通文本和尺寸文本的字体偏好。这样就可以在NX草图中使用自定义的字体了。
>
