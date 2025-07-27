### 【1065】dimension arcs 创建弧度尺寸

#### 代码

```cpp
    /*HEAD DIMENSION_ARCS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
    #include <uf_disp.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中新增的功能，用于打印系统日志。具体来说，该功能可以按照不同的日志级别打印系统日志。 */  
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
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_drf_arcs, NULL, &resp,  
            &arc->object_tag, cp, &arc->object_view_tag));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(arc->object_tag, 0));  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static logical select_pos(char *prompt, double pos[3])  
    {  
        int  
            resp = uc1615(prompt, pos);  
        if (resp == 5) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            dim;  
        double  
            horiz[3],  
            vert[3];  
        UF_DRF_object_t  
            one,  
            two;  
        UF_DRF_text_t  
            nada = { NULL, 0, NULL };  
        UF_CALL(UF_DRF_init_object_structure(&one));  
        one.object_assoc_type = UF_DRF_end_point;  
        one.object_assoc_modifier = UF_DRF_first_end_point;  
        UF_CALL(UF_DRF_init_object_structure(&two));  
        two.object_assoc_type = UF_DRF_tangency;  
        two.object_assoc_modifier = 0;  
        while (select_drf_arc("Select arc to dim", &one) &&  
                select_pos("Select vert dim origin", vert) &&  
                select_pos("Select horiz dim origin", horiz))  
        {  
            two.object_tag = one.object_tag;  
            two.object_view_tag = one.object_view_tag;  
            UF_CALL(UF_DRF_create_vertical_dim(&one, &two, &nada, vert, &dim));  
            one.object_assoc_modifier = UF_DRF_last_end_point;  
            two.object_assoc_modifier = 100;  
            UF_CALL(UF_DRF_create_horizontal_dim(&one, &two, &nada, horiz, &dim));  
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

> 这是段用于在NX中添加弧形尺寸的二次开发代码，其主要功能是添加垂直和水平尺寸到选择的弧。以下是该代码的主要逻辑：
>
> 1. 初始化: 初始化NX环境，定义错误报告函数。
> 2. 选择弧: 提示用户选择一个弧，并获取其对象标签和视图标签。
> 3. 选择尺寸原点: 提示用户选择垂直尺寸和水平尺寸的原点。
> 4. 添加尺寸: 使用获取的弧和尺寸原点，调用UF_DRF_create_vertical_dim和UF_DRF_create_horizontal_dim函数来添加垂直和水平尺寸。
> 5. 循环: 重复上述过程，直到用户取消选择。
> 6. 终止: 释放NX环境。
>
> 该代码通过交互式选择弧和尺寸原点，实现了快速添加弧形尺寸的功能。其中使用了NX提供的UF_DRF接口来创建尺寸，并通过UF_UI接口进行交互式选择。整体结构清晰，功能明确。
>
