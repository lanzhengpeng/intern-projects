### 【2030】report component of drafting curve with exact views 查询草图的精确视图组成部分

#### 代码

```cpp
    /*HEAD REPORT_COMPONENT_OF_DRAFTING_CURVE_WITH_EXACT_VIEWS CCC UFUN */  
    /*   
    Updated Sample to support the 'Exact' view types introduced in NX8.5.  
    Code requires at least NX9.0.2 MP1 or NX9.0.3 to use the new functions  
    (Please refer to PR-2237846 for details)  
    uf_draw.h:  
        - UF_DRAW_ask_drafting_curve_type()  
        - UF_DRAW_ask_drafting_curve_parents()  
    uf_draw_types:  
        - UF_DRAW_drafting_curve_type_t  
    */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_draw.h>  
    #include <uf_so.h>  
    #include <uf_assem.h>  
    #include <stdarg.h>  
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
            n_parents;  
        tag_t  
            comp = NULL_TAG,  
            *parents;  
        UF_DRAW_drafting_curve_type_t  
            curve_type;  
        ECHO("\nAsk Component of Drafting Curve %d\n", curve);  
        if (UF_ASSEM_is_occurrence(curve))  
        {  
            ECHO("Getting Component from Curve Occurrence\n");  
            comp = UF_ASSEM_ask_part_occurrence(curve);  
        }  
        else  
        {  
            UF_CALL( UF_DRAW_ask_drafting_curve_type(curve, &curve_type) );  
            switch(curve_type)  
            {  
            case UF_DRAW_unknown_type: ECHO("UF_DRAW_unknown_type\n"); break;  
            case UF_DRAW_extracted_edge_type: ECHO("UF_DRAW_extracted_edge_type\n"); break;  
            case UF_DRAW_silhouette_curve_type: ECHO("UF_DRAW_silhouette_curve_type\n"); break;  
            case UF_DRAW_thread_silhouette_curve_type: ECHO("UF_DRAW_thread_silhouette_curve_type\n"); break;  
            case UF_DRAW_section_edge_type: ECHO("UF_DRAW_section_edge_type\n"); break;  
            case UF_DRAW_thread_section_edge_type: ECHO("UF_DRAW_thread_section_edge_type\n"); break;  
            case UF_DRAW_vi_curve_type: ECHO("UF_DRAW_vi_curve_type\n"); break;  
            case UF_DRAW_uvhatch_curve_type: ECHO("UF_DRAW_uvhatch_curve_type\n"); break;  
            case UF_DRAW_trace_line_type: ECHO("UF_DRAW_trace_line_type\n"); break;  
            case UF_DRAW_simplified_curve_type: ECHO("UF_DRAW_simplified_curve_type\n"); break;  
            case UF_DRAW_interference_curve_type: ECHO("UF_DRAW_interference_curve_type\n"); break;  
            case UF_DRAW_extracted_model_curve_type: ECHO("UF_DRAW_extracted_model_curve_type\n"); break;  
            default: ECHO("Code should never come here!\n"); break;  
            };  
            UF_CALL( UF_DRAW_ask_drafting_curve_parents(curve, &n_parents, &parents) );  
            ECHO("UF_DRAW_ask_drafting_curve_parents returned %d parents\n", n_parents);  
            if (n_parents > 0)  
            {  
                if (UF_ASSEM_is_occurrence(parents[0]))  
                {  
                    ECHO("Getting Component from Parent Occurrence\n");  
                    comp = UF_ASSEM_ask_part_occurrence(parents[0]);  
                }  
                UF_free(parents);  
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
            component[MAX_FSPEC_SIZE+1];  
        while ((curve = select_drf_curve("Select curve to query")) != NULL_TAG)  
        {  
            if ((comp = ask_component_of_drafting_curve(curve)) != NULL_TAG)  
            {  
                UF_CALL(UF_DISP_set_highlight(comp, TRUE));  
                UF_ASSEM_ask_part_name_of_child(  
                    UF_ASSEM_ask_inst_of_part_occ(comp), component);  
                ECHO(" -> curve is from the component %s.\n", component);  
                UF_CALL(UF_DISP_set_highlight(comp, FALSE));  
            }  
            else  
            {  
                UF_CALL(UF_DISP_set_highlight(curve, TRUE));  
                ECHO(" -> curve is not from a component.\n");  
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

> 这段代码是基于NX的二次开发代码，主要功能是查询草绘曲线所属的组件。具体步骤如下：
>
> 1. 用户通过选择对话框选择一条草绘曲线。
> 2. 调用UF_DRAW_ask_drafting_curve_type函数获取草绘曲线的类型。
> 3. 调用UF_DRAW_ask_drafting_curve_parents函数获取草绘曲线的父对象。
> 4. 判断草绘曲线是否来自组件：如果曲线来自组件，则调用UF_ASSEM_ask_part_occurrence函数获取组件。如果曲线没有父对象，则直接显示曲线没有来自组件。
> 5. 如果曲线来自组件，则调用UF_ASSEM_ask_part_occurrence函数获取组件。
> 6. 如果曲线没有父对象，则直接显示曲线没有来自组件。
> 7. 对获取的组件进行高亮显示，并显示组件名称。
> 8. 重复选择，直到用户取消选择。
>
> 代码使用了UF_DRAW新引入的函数，用于获取草绘曲线的类型和父对象。同时，使用了UF_ASSEM函数来处理组件和实例。整体上，这段代码实现了根据草绘曲线查询所属组件的功能。
>
