### 【2659】select drf curve in view 在视图中选择DRF曲线

#### 代码

```cpp
    /* 里海 */  
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
    static tag_t select_drf_curve_in_view(char *prompt, tag_t *view)  
    {  
        int  
            resp;  
        tag_t  
            object;  
        double  
            cp[3];  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_drf_curves, NULL, &resp,  
            &object, cp, view));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 根据代码内容，这段代码主要用于在NX中实现曲线选择的功能。
>
> 主要功能包括：
>
> 1. 定义了一个选择掩码，用于限制在选择对话框中只能选择特定的曲线类型，如线、圆、圆锥曲线、样条线、实体边、实体轮廓和截面线。
> 2. 定义了一个函数select_drf_curve_in_view，用于在指定视图中选择曲线。函数首先清除当前的选择，然后设置选择掩码，打开选择对话框，等待用户选择一个曲线，最后返回用户选择的曲线的tag。
> 3. 在选择完成后，关闭高亮显示。
>
> 总的来说，这段代码通过定义选择掩码和选择函数，实现了在NX中选择特定类型曲线的功能，为后续的二次开发提供了基础。
>
