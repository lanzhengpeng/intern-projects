### 【2644】select curves or edges 选择曲线或边缘

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_curves_and_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[5] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 5, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_curves_or_edges(char *prompt, tag_t **curves)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select curves or edges", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_curves_and_edges, NULL, &resp, &cnt, curves));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*curves)[ii], FALSE));  
        return cnt;  
    }

```

#### 代码解析

> 这段代码是用于NX的二次开发，主要功能是定义一个选择过滤器，并使用该过滤器来选择曲线或边。
>
> 代码的主要部分包括：
>
> 1. 选择过滤器定义：mask_for_curves_and_edges函数定义了一个选择过滤器，允许选择直线、圆弧、样条线、实体边等类型的曲线或边。
> 2. 选择函数：select_curves_or_edges函数使用上述过滤器，弹出选择对话框，让用户选择曲线或边。它返回选择的元素数量和元素数组指针。
> 3. 取消高亮显示：在选择完成后，取消所有选择元素的临时高亮显示。
> 4. 返回选择结果：函数返回选择元素的数量和元素数组指针。
>
> 总体来说，这段代码定义了一个过滤器，实现了曲线或边的选择功能，并以结构化的方式返回了选择结果，方便后续处理。
>
