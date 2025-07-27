### 【2475】report selected objects color name 报告选定对象的颜色名称

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
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
    /* qq3123197280 */  
    static int mask_add_faces_and_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_anything(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog( "Select an object", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_add_faces_and_edges, NULL, &resp,  
            &object, cp, &view));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
    }  
    /*  Prior to NX 4.0, the returned full color name looked like this:  
        full_color_name = "2 (Green)"  
        So the method shown here cleans it up and returns just the name.  
    */  
    static void ask_color_name(int color_num, char *color_name)  
    {  
        double  
            clr_values[3];  
        char  
            *full_color_name;  
        UF_CALL(UF_DISP_ask_color(color_num, UF_DISP_rgb_model, &full_color_name,  
            clr_values));  
        if (strchr(full_color_name, '('))  
        {  
            strcpy(color_name, strchr(full_color_name, '(')+1);  
            strcpy(strchr(color_name, ')'), "");  
        }  
        else  
            strcpy(color_name, full_color_name);  
        UF_free(full_color_name);  
    }  
    static void ask_object_color_name(tag_t obj, char *color)  
    {  
        logical  
            is_disp;  
        UF_OBJ_disp_props_t  
            disp_props;  
        UF_CALL(UF_OBJ_is_displayable(obj, &is_disp));  
        if (is_disp)  
        {  
            UF_CALL(UF_OBJ_ask_display_properties(obj, &disp_props));  
            ask_color_name(disp_props.color, color);  
        }  
        else strcpy(color, "NOT DISPLAYABLE");  
    }  
    static void do_it(void)  
    {  
        tag_t  
            object;  
        char  
            color_name[MAX_ENTITY_NAME_SIZE+1];  
        while ((object = select_anything("Report Object Color Name")) != NULL_TAG)  
        {  
            ask_object_color_name(object, color_name);  
            WRITE_S(color_name);  
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

> 这段代码是用于NX的二次开发，主要功能是让用户选择NX模型中的任意对象，并输出该对象的颜色名称。以下是代码的主要组成部分：
>
> 1. 错误报告函数：report_error函数用于输出错误信息，包括文件名、行号、调用函数和错误码。
> 2. 选择函数：select_anything函数用于弹出一个选择对话框，让用户选择模型中的任意对象，并返回该对象的tag。它使用了mask_add_faces_and_edges函数来设置选择掩码，允许选择面和边。
> 3. 颜色名称查询函数：ask_color_name函数用于查询颜色编号对应的全名，ask_object_color_name函数用于查询对象的颜色名称，如果对象不可显示，则返回"NOT DISPLAYABLE"。
> 4. 主体函数：do_it函数循环调用select_anything函数，获取用户选择的对象，然后调用ask_object_color_name函数查询其颜色名称，并输出。
> 5. 入口函数：ufusr是程序的入口函数，用于初始化和终止NX，并调用do_it函数。
> 6. 卸载函数：ufusr_ask_unload函数返回立即卸载标志。
>
> 总的来说，这段代码通过选择对话框让用户选择对象，并查询其颜色名称，实现了简单的颜色查询功能。
>
