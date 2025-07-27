### 【0788】create note associated to selected arc center 创建与所选弧中心相关的注释

#### 代码

```cpp
    /*HEAD CREATE_NOTE_ASSOCIATED_TO_SELECTED_ARC_CENTER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
    #include <uf_so.h>  
    #include <uf_curve.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_draw.h>  
    #include <uf_view.h>  
    #include <uf_part.h>  
    #include <uf_eval.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：UF_print_syslog是在V18中新引入的，只回答译文，不要无关内容。 */  
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
    /* 里海 */  
    static int mask_for_drf_arcs(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[3] = {{ UF_circle_type, 0, 0 },  
                       {UF_solid_type, 0, UF_UI_SEL_FEATURE_CIRCULAR_EDGE },  
                       { UF_section_edge_type, 0, 0 }};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 3, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static logical select_drf_arc(char *prompt, UF_DRF_object_p_t arc)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_drf_arcs, NULL, &resp,  
            &arc->object_tag, cp, &arc->object_view_tag));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(arc->object_tag, 0));  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            a_note;  
        double  
            dwg_loc[2],  
            zero[3] = { 0,0,0 };  
        char  
            *text[1] = { "NOTE ALWAYS 0.5 ABOVE ARC CENTER" };  
        UF_EVAL_p_t  
            evaluator;  
        UF_EVAL_arc_t  
            e_arc;  
        UF_DRF_object_t  
            arc;  
        UF_DRF_associative_origin_t  
            origin;  
        origin.origin_type = UF_DRF_ORIGIN_RELATIVE_TO_GEOMETRY;  
        while (select_drf_arc("Select arc for note", &arc))  
        {  
            UF_CALL(UF_DRF_create_note(1, text, zero, 0, &a_note));  
            UF_CALL(UF_EVAL_initialize(arc.object_tag, &evaluator));  
            UF_CALL(UF_EVAL_ask_arc(evaluator, &e_arc));  
            UF_CALL(UF_EVAL_free(evaluator));  
            UF_CALL(UF_VIEW_map_model_to_drawing(arc.object_view_tag, e_arc.center,  
                dwg_loc));  
            UF_CALL(UF_SO_create_point_conic_center(arc.object_tag,  
                UF_SO_update_after_modeling, arc.object_tag,  
                &origin.point_on_geometry));  
            origin.view_of_geometry = arc.object_view_tag;  
            dwg_loc[1] = dwg_loc[1] + 0.5;  
            UF_CALL(UF_DRF_set_associative_origin(a_note, &origin, dwg_loc));  
        }  
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

> 根据代码，这是一个用于在NX中创建与所选弧线中心相关的注释的二次开发代码。主要功能包括：
>
> 1. 错误报告机制：定义了report_error函数，用于在UF函数调用失败时报告错误信息。
> 2. 选择弧线：定义了mask_for_drf_arcs和select_drf_arc函数，用于设置选择掩码并选择一个弧线。
> 3. 创建注释：在do_it函数中，首先创建一个注释，然后通过UF_EVAL获取弧线中心点，并通过UF_VIEW映射到图纸坐标系。
> 4. 关联注释：使用UF_DRF_set_associative_origin将注释关联到弧线中心，并设置注释在图纸坐标系中的位置。
> 5. 主函数：在ufusr函数中初始化UF，调用do_it函数，然后终止UF。
> 6. 卸载函数：定义了ufusr_ask_unload函数，用于立即卸载用户程序。
>
> 整体来看，这段代码实现了选择弧线并创建与弧线中心相关的注释的功能，并具有错误处理机制。
>
