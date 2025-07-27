### 【2723】set all notes font 设置所有注释字体

#### 代码

```cpp
    /*HEAD SET_ALL_NOTES_FONT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_ugfont.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是在 V18 版本中新增的函数。它用于在日志文件中输出信息。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    static tag_t ask_next_note(tag_t part, tag_t a_note)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drafting_entity_type,  
            &a_note)) && (a_note != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(a_note, &type, &subtype));  
            if (subtype == UF_draft_note_subtype) return a_note;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            mpi[100],  
            n;  
        tag_t  
            note = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        double  
            mpr[70];  
        char  
            rad[27],  
            dia[27];  
        WRITE_D(n = ask_font_index_number("leroy"));  
        while ((note = ask_next_note(part, note)) != NULL_TAG)  
        {  
            UF_CALL(UF_DRF_ask_object_preferences(note, mpi, mpr, rad, dia));  
            mpi[88] = n;  
            UF_CALL(UF_DRF_set_object_preferences(note, mpi, mpr, rad, dia));  
        }  
    }  
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

> 这段NX Open C++代码实现了设置所有注释字体为Leroy字体的功能。
>
> 主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于在发生错误时输出错误信息和调用堆栈。
> 2. 定义了一个写整数到列表窗口的函数write_integer_to_listing_window，用于输出调试信息。
> 3. 实现了获取字体表实体的函数ask_font_table_entity，以及根据字体名称获取字体索引号的函数ask_font_index_number。
> 4. 实现了获取下一个注释实体的函数ask_next_note。
> 5. 主函数do_it遍历所有注释实体，调用UF_DRF_ask_object_preferences和UF_DRF_set_object_preferences函数获取和设置注释实体的对象首选项，将字体索引号设置为Leroy字体的索引号。
> 6. ufusr函数是二次开发的入口函数，初始化环境后调用do_it，最后终止环境。
> 7. ufusr_ask_unload函数用于卸载二次开发，返回立即卸载的标志。
>
> 该代码通过遍历注释实体并设置字体索引号，实现了批量设置注释字体为Leroy字体的功能，可用于提高注释的可读性。
>
