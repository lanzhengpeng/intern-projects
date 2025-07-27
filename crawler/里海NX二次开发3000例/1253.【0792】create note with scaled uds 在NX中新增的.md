### 【0792】create note with scaled uds 在NX中新增的

#### 代码

```cpp
    /*HEAD CREATE_NOTE_WITH_SCALED_UDS CCC UFUN */  
    /* new in NX 译:翻译为：

在NX中新增的。 */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
    #include <uf_drf_types.h>  
    #include <uf_ugfont.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog 是 V18 新增的功能。 */  
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
    static logical prompt_for_two_numbers(char *prompt, char *item1, char *item2,  
        double *number1, double *number2)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[2][16];  
        double  
            da[2];  
        strncpy(&menu[0][0], item1, 15);  
        strncpy(&menu[1][0], item2, 15);  
        da[0] = *number1;  
        da[1] = *number2;  
        resp = uc1609(prompt, menu, 2, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = da[0];  
            *number2 = da[1];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            the_note[2];  
        char  
            *text1[1] = { "Embedded User Defined Symbol: <%CLINE>, for example."};  
        double  
            scale = 2.0,  
            aspect = 1.0,  
            origin1[3] = { 0, 0, 0 };  
        UF_DRF_lettering_preferences_t  
            lettering_preferences;  
        UF_DRF_uds_size_t   
            uds_size = { UF_DRF_SCALE_ASPECT_RATIO, 2.0, 1.0};  
        /* Could use UF_DRF_LENGTH_HEIGHT, instead, if desired 译:如果您愿意，可以使用UF_DRF_LENGTH_HEIGHT。 */  
        prompt_for_two_numbers("Enter UDS Parameters", "Scale", "Aspect",   
            &scale, &aspect);  
        UF_CALL(UF_DRF_ask_lettering_preferences(&lettering_preferences));  
        lettering_preferences.general_text.cfw.font =  
            ask_font_index_number("leroy");  
        UF_CALL(UF_DRF_set_lettering_preferences(&lettering_preferences));  
        uds_size.length_or_scale = scale;  
        uds_size.height_or_aspect_ratio = aspect;  
        UF_DRF_set_uds_size(&uds_size);  
        UF_CALL(UF_DRF_create_note(1, text1, origin1, 0, &the_note[0]));  
    }  
    /* 里海 */  
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

> 这段代码是一个NX的二次开发示例，其主要功能是创建一个具有缩放和宽高比的用户自定义符号(UDS)。以下是代码的主要功能和步骤：
>
> 1. 包含必要的NX头文件，包括UF、UI、DRF等库的头文件。
> 2. 定义了一个错误报告函数report_error，用于在调用NX API失败时输出错误信息。
> 3. 定义了ask_font_table_entity函数，用于查询当前显示部件的字体表实体。
> 4. 定义了ask_font_index_number函数，用于向字体表中添加字体并获取其索引号。
> 5. 定义了prompt_for_two_numbers函数，用于弹出一个对话框提示用户输入两个数字，并返回用户输入的结果。
> 6. 定义了do_it函数，这是主函数，执行以下步骤：获取当前的字母标注偏好设置lettering_preferences。修改lettering_preferences中的字体为"leroy"。设置缩放和宽高比参数uds_size。创建一个文本标注，内容为"Embedded User Defined Symbol: <%CLINE>"，并设置其位置为原点(0,0,0)。
> 7. 获取当前的字母标注偏好设置lettering_preferences。
> 8. 修改lettering_preferences中的字体为"leroy"。
> 9. 设置缩放和宽高比参数uds_size。
> 10. 创建一个文本标注，内容为"Embedded User Defined Symbol: <%CLINE>"，并设置其位置为原点(0,0,0)。
> 11. 定义了ufusr函数，这是NX二次开发程序的入口函数。在初始化NX环境后调用do_it函数，执行创建UDS标注的逻辑，最后终止NX环境。
> 12. 定义了ufusr_ask_unload函数，用于在卸载NX二次开发程序时立即卸载。
>
> 综上所述，这段代码的主要功能是创建一个具有自定义字体、缩放和宽高比的用户自定义符号标注。通过二次开发，可以扩展NX的功能，实现更丰富的标注效果。
>
