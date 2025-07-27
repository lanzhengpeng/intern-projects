### 【2555】select a curve or edge 选择一个曲线或边缘

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
    extern tag_t select_a_curve_or_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve or edge", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves_and_edges, NULL,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发的函数，主要实现了在NX中选择曲线或边的功能。以下是代码的主要功能：
>
> 1. 定义了一个名为mask_for_curves_and_edges的函数，该函数为选择集设置选择掩码，以允许选择线、圆、圆锥曲线、样条线和实体边。
> 2. 定义了一个名为select_a_curve_or_edge的函数，该函数用于显示一个对话框，提示用户选择一个曲线或边。它使用mask_for_curves_and_edges函数设置选择掩码，并获取用户的选择结果。
> 3. 如果用户选择了对象，则取消高亮显示，并返回所选对象的标签；否则，返回空标签。
> 4. 在select_a_curve_or_edge函数中，使用UF_UI_select_with_single_dialog函数显示选择对话框，并获取用户的选择响应、对象标签、视图等信息。
> 5. 通过检查响应代码，判断用户是否成功选择了对象，然后进行相应的处理。
> 6. 使用UF_DISP_set_highlight函数取消高亮显示所选对象。
>
> 总体来说，这段代码通过自定义选择掩码，实现了一个选择曲线或边的通用函数，用户可以通过调用该函数来实现选择操作。
>
