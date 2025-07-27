### 【2706】select single curve 选择单条曲线

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_curves(UF_UI_selection_p_t select, void *user_data)  
    {  
        UF_UI_mask_t  
            mask[5] = { UF_line_type, 0, 0,  
                        UF_circle_type, 0, 0,  
                        UF_conic_type, 0, UF_all_subtype,  
                        UF_spline_type, 0, 0,  
                        UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE };  
        if (UF_CALL(UF_UI_set_sel_mask(select,  
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 5, mask)))  
            return (UF_UI_SEL_FAILURE);  
        else  
            return (UF_UI_SEL_SUCCESS);  
    }  
    static tag_t select_single_curve(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            curve,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( "Select curve", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_curves, NULL, &resp, &curve, cp,  
            &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(curve, 0));  
            return curve;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是用于NX软件的二次开发，其目的是实现选择单一曲线的功能。主要包含两个函数：
>
> 1. mask_for_curves函数：该函数用于设置选择集的选择掩码，仅允许选择线、圆、圆锥曲线、样条线以及实体类型的边。这样保证了用户只能选择单一曲线。
> 2. select_single_curve函数：该函数通过调用NX的UI函数，使用单选对话框提示用户选择曲线。在用户选择后，会获取选择的曲线tag，并设置高亮显示。如果用户成功选择了一条曲线，则返回该曲线的tag，否则返回NULL_TAG。
>
> 这两个函数配合使用，实现了NX软件中单一曲线的选择功能，为后续的曲线处理提供了基础。
>
