### 【2649】select dimensions in member views 在成员视图中选择尺寸

#### 代码

```cpp
    /*HEAD SELECT_DIMENSIONS_IN_MEMBER_VIEWS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_view.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中新增的功能。

仅提供翻译，不要添加其他内容。 */  
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
    static int mask_for_dimensions(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_dimension_type, UF_all_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_dimensions(char *prompt, tag_t **objects)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_class_dialog("Select Dimensions", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_dimensions, NULL, &resp, &cnt,  
            objects));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*objects)[ii], 0));  
        return cnt;  
    }  
    static tag_t ask_object_view_tag(tag_t object)  
    {  
        int  
            status;  
        tag_t  
            view = NULL_TAG;  
        char  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(uc6409(object, &status, view_name));  
        if (status == 1)  
            UF_CALL(UF_VIEW_ask_tag_of_view_name(view_name, &view));  
        return view;  
    }  
    static void display_temporary_colored_point(double *coords, int color)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        attrib.color = color;  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_dims;  
        tag_t  
            *dims,  
            view,  
            work_view;  
        double  
            dwg_pt[3] = { 0, 0, 0 },  
            origin[3];  
        UF_CALL(UF_VIEW_ask_work_view(&work_view));  
        while ((n_dims = select_dimensions("Select dimensions", &dims)) > 0)  
        {  
            for (ii = 0; ii < n_dims; ii++)  
            {  
                view = ask_object_view_tag(dims[ii]);  
                UF_CALL(UF_DRF_ask_origin(dims[ii], origin));  
                if (view != work_view)  
                {  
                    UF_CALL(UF_VIEW_map_model_to_drawing(view, origin, dwg_pt));  
                    display_temporary_colored_point(dwg_pt, UF_OBJ_RED);  
                }  
                else  
                    display_temporary_colored_point(origin, UF_OBJ_GREEN);  
            }  
            UF_free(dims);  
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

> 这是段NX二次开发代码，用于在NX中实现选择标注，并根据标注所在视图显示不同颜色的临时点。主要功能包括：
>
> 1. 使用UF_UI模块选择标注，并设置选择掩码只选择标注。
> 2. 遍历所有选中的标注，使用ask_object_view_tag函数获取标注所在的视图tag。
> 3. 使用UF_DRF模块获取标注的原点坐标。
> 4. 判断标注所在的视图是否为当前工作视图，如果是，则在原点显示绿色的临时点；如果不是，则将原点坐标从模型空间转换到视图空间，并显示红色的临时点。
> 5. 通过循环调用选择标注的函数，直到用户没有选择任何标注为止。
> 6. 通过UF_DISP模块显示临时点，使用不同的颜色表示不同视图的标注。
> 7. 通过UF_VIEW模块进行模型空间到视图空间的坐标转换。
> 8. 使用UF_CALL宏封装UF函数调用，并报告错误。
> 9. 在ufusr函数中初始化和终止NX Open API。
> 10. 定义了卸载函数返回立即卸载。
>
> 以上是对该代码的主要功能的介绍。
>
