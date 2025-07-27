### 【2031】report component of drafting curve 报告草绘曲线的组件

#### 代码

```cpp
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_draw.h>  
    #include <uf_so.h>  
    #include <uf_assem.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
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
    static int mask_for_drf_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[7] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE },  
                        { UF_solid_silhouette_type, 0, 0 },  
                        { UF_section_edge_type, 0, 0 }};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 7, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_drf_curve(char *prompt)  
    {  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        double  
            cp[3];  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_drf_curves, NULL, &resp,  
            &object, cp, &view));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static tag_t ask_component_of_drafting_curve(tag_t curve)  
    {  
        int  
            gtype,  
            n_parents,  
            subtype;  
        tag_t  
            comp = NULL_TAG,  
            group,  
            *parents;  
        if (UF_ASSEM_is_occurrence(curve))  
            comp = UF_ASSEM_ask_part_occurrence(curve);  
        else  
        {  
            UF_CALL(UF_DRAW_ask_group_of_curve(curve, &group, &gtype, &subtype));  
            if ((group != NULL_TAG) && ((gtype == UF_solid_section_type) ||  
                    (gtype == UF_solid_silhouette_type) ||  
                    (gtype == UF_curve_group_type)))  
            {  
                UF_CALL(UF_SO_ask_parents(curve, UF_SO_ASK_ALL_PARENTS, &n_parents,  
                    &parents));  
                if (n_parents > 0)  
                {  
                    if (UF_ASSEM_is_occurrence(parents[0]))  
                        comp = UF_ASSEM_ask_part_occurrence(parents[0]);  
                    UF_free(parents);  
                }  
            }  
        }  
        return comp;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            curve,  
            comp;  
        char  
            component[MAX_FSPEC_SIZE+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        while ((curve = select_drf_curve("Select curve to query")) != NULL_TAG)  
        {  
            if ((comp = ask_component_of_drafting_curve(curve)) != NULL_TAG)  
            {  
                UF_CALL(UF_DISP_set_highlight(comp, TRUE));  
                UF_ASSEM_ask_part_name_of_child(  
                    UF_ASSEM_ask_inst_of_part_occ(comp), component);  
                sprintf(msg, "curve is from %s", component);  
                uc1601(msg, TRUE);  
                UF_CALL(UF_DISP_set_highlight(comp, FALSE));  
            }  
            else  
            {  
                UF_CALL(UF_DISP_set_highlight(curve, TRUE));  
                uc1601("curve is not part of a component", TRUE);  
                UF_CALL(UF_DISP_set_highlight(curve, FALSE));  
            }  
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

> 这段代码是一个NX二次开发示例，其主要功能包括：
>
> 1. 错误报告和日志输出：使用了宏定义UF_CALL来封装NX API调用，并在出错时输出错误信息到日志窗口和系统日志。
> 2. 曲线选择对话框：定义了mask_for_drf_curves函数，用于设置选择曲线时的过滤条件，然后调用UF_UI_select_with_single_dialog函数弹出一个曲线选择对话框，使用户选择一个曲线。
> 3. 获取曲线所在组件：定义了ask_component_of_drafting_curve函数，用于获取用户选择的曲线所在的组件。如果曲线是组件实例，则直接获取组件；如果曲线属于某个曲线组，则通过曲线组的父级实体获取组件。
> 4. 组件高亮和提示：在do_it函数中，使用一个循环，让用户反复选择曲线，获取每个曲线所在的组件，并高亮显示组件，同时弹出提示框显示组件名称。
> 5. 初始化和结束：在ufusr函数中，首先调用UF_initialize进行初始化，然后调用do_it执行主体功能，最后调用UF_terminate结束。
> 6. 卸载函数：定义了ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY表示可以立即卸载该二次开发程序。
>
> 综上所述，这段代码实现了一个简单的交互式曲线查询组件名称的NX二次开发程序。
>
