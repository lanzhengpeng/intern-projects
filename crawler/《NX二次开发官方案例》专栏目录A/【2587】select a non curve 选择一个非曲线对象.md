### 【2587】select a non curve 选择一个非曲线对象

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_no_curves(UF_UI_selection_p_t select, void *user_data)  
    {  
        UF_UI_mask_t  
            mask[5] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, UF_all_subtype, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (UF_CALL(UF_UI_set_sel_mask(select,  
            UF_UI_SEL_MASK_ALL_AND_DISABLE_SPECIFIC, 5, mask)))  
            return (UF_UI_SEL_FAILURE);  
        else  
            return (UF_UI_SEL_SUCCESS);  
    }  
    static tag_t select_a_non_curve(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select object (not a curve)",  
            prompt, UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_no_curves, NULL,  
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

> 这段代码是用于在NX中实现选择非曲线对象的功能。
>
> 首先，定义了一个数组mask，它包含了5个元素，每个元素都指定了不希望被选择的对象类型。这包括直线、圆、圆锥曲线、样条曲线以及实体边。
>
> 接着，定义了一个名为mask_for_no_curves的函数，它接收选择集和用户数据两个参数。该函数通过调用UF_UI_set_sel_mask函数，将mask数组应用到选择集中，以屏蔽掉不希望被选择的对象类型。
>
> 然后，定义了一个名为select_a_non_curve的函数，它用于提示用户选择一个非曲线对象。函数首先提示用户进行选择，然后调用UF_UI_select_with_single_dialog函数进行选择。如果用户选择了对象，则返回该对象的tag；如果没有选择，则返回NULL_TAG。
>
> 总的来说，这段代码通过屏蔽掉不需要的对象类型，实现了在NX中选择非曲线对象的功能。
>
