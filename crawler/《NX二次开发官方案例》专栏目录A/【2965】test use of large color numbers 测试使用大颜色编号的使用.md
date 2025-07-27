### 【2965】test use of large color numbers 测试使用大颜色编号的使用

#### 代码

```cpp
    /*HEAD TEST_USE_OF_LARGE_COLOR_NUMBERS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_drf.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据原文，UF_print_syslog是V18版本新增的功能。 */  
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
        logical  
            is_def;  
        int  
            old_def;  
        tag_t  
            line,  
            a_note,  
            point;  
        double  
            org[3] = { 0, 0, 0 };  
        char  
            *text[1] = { "TEXT" };  
        UF_OBJ_cre_settings_t  
            old_pref;  
        UF_CURVE_line_t  
            line_data = { {0,0,0}, {1,0,0} };  
    /*  Save the original note color preference 里海译:保留原始便签颜色偏好 */  
        UF_CALL(UF_OBJ_ask_cre_settings(UF_drafting_entity_type,  
            UF_draft_note_subtype, UF_OBJ_no_property, &old_pref));  
    /*  Set the desired note color preference 里海译:设置所需的音符颜色偏好 */  
        UF_CALL(UF_OBJ_set_cre_color(UF_drafting_entity_type,  
            UF_draft_note_subtype, UF_OBJ_no_property, 201));  
        UF_CALL(UF_DRF_create_note(1, text, org, 0, &a_note));  
    /*  Reset the original note color preference 里海译:重置原始笔记颜色偏好 */  
        UF_CALL(UF_OBJ_set_cre_color(UF_drafting_entity_type,  
            UF_draft_note_subtype, UF_OBJ_no_property, old_pref.color));  
    /*  Save the original default color preference 里海译:保存原始默认颜色偏好 */  
        UF_CALL(UF_OBJ_ask_def_cre_settings(&old_pref));  
        old_def = old_pref.color;  
    /*  Set the desired default color 里海译:设置期望的默认颜色 */  
        UF_CALL(UF_OBJ_set_def_cre_color(115));  
    /*  Be sure lines are set to create in the default color 里海译:确保线条设置为默认颜色。 */  
        UF_CALL(UF_OBJ_is_def_cre_color(UF_line_type, UF_all_subtype,  
            UF_OBJ_no_property, &is_def));  
        if (!is_def)  
        {  
            UF_CALL(UF_OBJ_ask_cre_settings(UF_line_type, UF_all_subtype,  
                UF_OBJ_no_property, &old_pref));  
            UF_CALL(UF_OBJ_set_cre_color_to_def(UF_line_type, UF_all_subtype,  
                UF_OBJ_no_property));  
        }  
        UF_CALL(UF_CURVE_create_line(&line_data, &line));  
    /*  Reset the original line color preference 里海译:重置原始线条颜色偏好 */  
        if (!is_def)  
            UF_CALL(UF_OBJ_set_cre_color(UF_line_type, UF_all_subtype,  
                UF_OBJ_no_property, old_pref.color));  
    /*  Reset the original default color 里海译:重置原始默认颜色 */  
        UF_CALL(UF_OBJ_set_def_cre_color(old_def));  
        org[1] = 1;  
        UF_CALL(UF_CURVE_create_point(org, &point));  
        UF_CALL(UF_OBJ_set_color(point, 143));  
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

> 这段代码是NX CAD系统的二次开发代码，主要功能是演示如何设置和恢复绘图实体的颜色属性。代码的关键步骤如下：
>
> 1. 使用UF_OBJ_ask_cre_settings函数获取创建设置，以保存原始便签颜色偏好。
> 2. 通过UF_OBJ_set_cre_color函数设置便签的创建颜色为201。
> 3. 使用UF_DRF_create_note函数创建一个便签。
> 4. 恢复便签的原始颜色设置。
> 5. 保存原始默认创建颜色。
> 6. 设置期望的默认创建颜色为115。
> 7. 确保线条使用默认颜色。
> 8. 创建一条线。
> 9. 恢复线条的原始颜色设置。
> 10. 恢复原始默认颜色。
> 11. 创建一个点，并设置其颜色为143。
> 12. 使用UF_print_syslog函数输出错误信息。
> 13. 提供UFusr和ufusr_ask_unload函数作为NX的二次开发入口。
>
> 总的来说，这段代码演示了如何通过NX提供的UF函数来设置和恢复绘图实体的颜色属性，从而实现颜色的灵活控制。
>
