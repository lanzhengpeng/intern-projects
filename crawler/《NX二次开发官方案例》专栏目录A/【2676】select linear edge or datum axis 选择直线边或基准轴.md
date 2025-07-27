### 【2676】select linear edge or datum axis 选择直线边或基准轴

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_linear_edges_or_datum_axes(UF_UI_selection_p_t select,  
        void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_LINEAR_EDGE },  
                        { UF_datum_axis_type, 0, 0 }};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_linear_edge_or_datum_axis(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select edge/datum axis", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_linear_edges_or_datum_axes,  
            NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能是选择直线边或基准轴。
>
> 代码首先定义了一个名为mask_for_linear_edges_or_datum_axes的函数，用于设置选择掩码，允许选择直线边和基准轴。
>
> 接着，定义了一个名为select_a_linear_edge_or_datum_axis的函数，用于显示选择对话框，让用户选择直线边或基准轴。如果用户成功选择了对象，则返回该对象的tag，否则返回NULL_TAG。
>
> 最后，使用UF_UI_select_with_single_dialog函数显示选择对话框，并调用mask_for_linear_edges_or_datum_axes函数设置选择掩码。如果用户选择了对象，则返回该对象的tag。
>
> 总体来说，这段代码通过自定义选择掩码和选择对话框，实现了在NX中精确选择直线边或基准轴的功能。
>
