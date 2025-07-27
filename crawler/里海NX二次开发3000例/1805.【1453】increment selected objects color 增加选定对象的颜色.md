### 【1453】increment selected objects color 增加选定对象的颜色

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
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
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_add_faces_and_edges, NULL,  
            &resp, &object, cp, &view));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static int ask_object_color(tag_t obj)  
    {  
        logical  
            is_disp;  
        UF_OBJ_disp_props_t  
            disp_props;  
        UF_CALL(UF_OBJ_is_displayable(obj, &is_disp));  
        if (is_disp)  
        {  
            UF_CALL(UF_OBJ_ask_display_properties(obj, &disp_props));  
            return disp_props.color;  
        }  
        else return 0;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            object;  
        while ((object = select_anything("Increment color")) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_set_color(object, ask_object_color(object) + 1));  
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

> 这段代码是NX的二次开发代码，主要功能是让用户选择一个对象，然后将其颜色增加1，从而实现颜色渐变的效果。代码的主要逻辑如下：
>
> 1. 定义了一个ECHO函数，用于在列表窗口和系统日志中输出信息，用于调试。
> 2. 定义了一个report_error函数，用于在函数调用失败时输出错误信息。
> 3. 定义了一个mask_add_faces_and_edges函数，用于设置选择过滤器，只选择实体面和边。
> 4. 定义了一个select_anything函数，用于弹出选择对话框，让用户选择一个对象，并返回对象的tag。
> 5. 定义了一个ask_object_color函数，用于获取一个对象的颜色属性。
> 6. 定义了一个do_it函数，用于循环调用select_anything选择对象，并调用ask_object_color获取颜色，然后调用UF_OBJ_set_color将颜色增加1。
> 7. 定义了ufusr函数，这是NX二次开发的入口函数，调用UF_initialize初始化环境，调用do_it执行主要功能，最后调用UF_terminate终止环境。
> 8. 定义了ufusr_ask_unload函数，用于在插件卸载时立即卸载。
>
> 总体来说，这段代码通过选择对象并修改颜色属性，实现了颜色渐变的效果。代码结构清晰，功能明确。
>
