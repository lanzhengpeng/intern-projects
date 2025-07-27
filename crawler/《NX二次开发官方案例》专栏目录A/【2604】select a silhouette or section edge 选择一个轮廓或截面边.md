### 【2604】select a silhouette or section edge 选择一个轮廓或截面边

#### 代码

```cpp
    /* 里海 */  
    static int filter_silhouettes_and_section_edges(tag_t object, int type[3],  
        void *data, UF_UI_selection_p_t select)  
    {  
        int  
            subtype,  
            gtype;  
        tag_t  
            group;  
        UF_CALL(UF_DRAW_ask_group_of_curve(object, &group, &gtype, &subtype));  
        if ((group != NULL_TAG) && ((gtype == UF_solid_section_type) ||  
            gtype == UF_solid_silhouette_type))  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    /* 里海 */  
    static int mask_for_silhouettes_and_section_edges(UF_UI_selection_p_t select,  
        void *type)  
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
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 7, mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select,  
                filter_silhouettes_and_section_edges, NULL, NULL)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_silhouette_or_section_edge(char *prompt)  
    {  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        double  
            cp[3];  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog(  
            "Select a silhouette or section edge", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_silhouettes_and_section_edges,  
            NULL, &resp, &object, cp, &view));  
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

> 这段代码是用于在NX CAD软件中进行二次开发的示例，主要实现了以下功能：
>
> 1. 定义了一个过滤函数 filter_silhouettes_and_section_edges，用于在用户选择时只接受轮廓线或截面线。
> 2. 定义了一个掩码函数 mask_for_silhouettes_and_section_edges，设置了允许选择的对象类型为线、圆、锥曲线、样条线、实体轮廓线和截面线。
> 3. 定义了一个选择函数 select_a_silhouette_or_section_edge，用于提示用户选择一个轮廓线或截面线，并返回所选对象。
> 4. 在选择函数中，首先设置光标视图，然后使用对话框提示用户选择，并设置掩码和过滤函数，最后返回用户选择的轮廓线或截面线对象。
> 5. 该代码实现了在NX中快速选择轮廓线或截面线对象的功能，可用于后续的二次开发操作，例如提取轮廓线或截面线的几何信息等。
>
