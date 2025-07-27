### 【1363】hide selected components 隐藏选中的组件

#### 代码

```cpp
    /*HEAD HIDE_SELECTED_COMPONENTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
    #include <uf_view.h>  
    #include <uf_disp.h>  
    #include <uf_draw.h>  
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
注意：UF_print_syslog 是 V18 中的新功能，只需回答翻译，不要添加无关内容。 */  
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
    /* qq3123197280 */  
    static int mask_for_components(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_component_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_component(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL( UF_UI_set_cursor_view( 0));  
        UF_CALL(UF_UI_select_with_single_dialog("Select a component", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_components, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static tag_t select_a_view(char *prompt)  
    {  
        int  
            view_of_cursor,  
            resp;  
        double  
            pos[3];  
        char  
            vname[MAX_ENTITY_NAME_SIZE+1];  
        tag_t  
            view = NULL_TAG;  
        UF_CALL( UF_UI_ask_cursor_view( &view_of_cursor));  
        UF_CALL( UF_UI_set_cursor_view( 0));  
        resp = uc1615(prompt, pos);  
        UF_CALL( UF_UI_set_cursor_view( view_of_cursor));  
        if (resp == 5)  
        {  
            UF_CALL( uc1653( vname ));  
            UF_CALL( UF_VIEW_ask_tag_of_view_name(vname, &view));  
            return view;  
        }  
        else  
            return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            comp,  
            dwg,  
            view;  
        UF_VIEW_subtype_t  
            subtype;  
        UF_VIEW_type_t  
            type;  
        while ((comp = select_a_component("Hide component")) != NULL_TAG)  
            while ((view = select_a_view("Hide in view")) != NULL_TAG)  
            {  
                UF_CALL(UF_ASSEM_hide_component(comp, view));  
                UF_CALL(UF_VIEW_ask_type(view, &type, &subtype));  
                if (type == UF_VIEW_DRAWING_MEMBER_TYPE)  
                {  
                    UF_CALL(UF_DRAW_ask_drawing_of_view(view, &dwg));  
                    UF_CALL(UF_DRAW_update_one_view(dwg, view));  
                }  
                else  
                    UF_CALL(UF_DISP_regenerate_view(view));  
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

> 这段NX二次开发代码主要实现了隐藏组件的功能。代码主要包含以下几个关键部分：
>
> 1. 错误报告函数report_error()：用于在函数执行出错时输出错误信息。
> 2. 组件选择函数select_a_component()：使用UI函数让用户选择要隐藏的组件。
> 3. 视图选择函数select_a_view()：使用UI函数让用户选择要隐藏组件的视图。
> 4. 主函数do_it()：循环调用上述两个函数，获取组件和视图的tag，然后调用UF_ASSEM_hide_component()隐藏组件。
> 5. ufusr()：NX二次开发入口函数，调用do_it()执行隐藏组件功能。
> 6. ufusr_ask_unload()：询问卸载函数，返回立即卸载。
>
> 总体而言，这段代码通过UI交互让用户选择组件和视图，然后隐藏指定的组件。隐藏组件后，根据视图类型，更新视图或者重新生成视图。
>
