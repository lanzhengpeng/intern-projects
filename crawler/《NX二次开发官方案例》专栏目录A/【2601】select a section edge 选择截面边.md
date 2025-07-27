### 【2601】select a section edge 选择截面边

#### 代码

```cpp
    static int filter_section_edges(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        int  
            subtype,  
            gtype;  
        tag_t  
            group;  
        UF_CALL(UF_DRAW_ask_group_of_curve(object, &group, &gtype, &subtype));  
        if ((group != NULL_TAG) && (gtype == UF_solid_section_type))  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    /* 里海 */  
    static int mask_for_section_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[6] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE },  
                        { UF_section_edge_type, 0, 0 }};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 6, mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_section_edges, NULL,NULL)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_section_edge(char *prompt)  
    {  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        double  
            cp[3];  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog("Select a section edge", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_section_edges, NULL, &resp,  
            &object, cp, &view));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是用于NX的二次开发，其主要功能是选择一个截面线。
>
> 代码的核心逻辑如下：
>
> 1. 定义了一个过滤函数filter_section_edges，用于过滤出截面线。它通过查询曲线的组信息，判断是否为实体截面线，如果是则接受选择，否则拒绝。
> 2. 定义了一个掩码函数mask_for_section_edges，用于设置选择器的掩码，只允许选择线、圆、圆锥、样条和实体边缘。
> 3. 定义了一个选择函数select_a_section_edge，用于执行选择操作。它首先设置光标视图，然后调用UF_UI_select_with_single_dialog函数，显示选择对话框，并调用掩码函数设置掩码，最终返回选中的对象。
> 4. 在选择完成后，取消高亮显示并返回选中对象。
>
> 总的来说，这段代码实现了在NX中通过二次开发选择一个实体截面线的功能。通过设置合适的掩码和过滤条件，实现了精确的选择控制。
>
