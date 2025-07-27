### 【2661】select drf curve 选择绘图曲线

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_drf_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[9] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE },  
                        { UF_section_line_type, 0, 0 },  
                        { UF_section_segment_type, 0, 0 },  
                        { UF_solid_silhouette_type, 0, 0 },  
                        { UF_section_edge_type, 0, 0 }};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 9, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_drf_curve(char *prompt)  
    {  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        double  
            cp[3];  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_drf_curves, NULL, &resp,  
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

> 这段代码是NX Open API的一个示例，用于选择一个绘图曲线。
>
> 首先，定义了一个名为mask_for_drf_curves的函数，用于设置选择掩码，只允许选择线、圆、二次曲线、样条线、实体边缘、截面线、截面线段、实体轮廓和截面边。然后，定义了一个名为select_drf_curve的函数，用于使用单选对话框让用户选择一个绘图曲线。
>
> 在select_drf_curve函数中，首先将光标视图设置为0，即不显示光标。然后调用UF_UI_select_with_single_dialog函数，让用户在单选对话框中选择一个绘图曲线。选择完成后，将光标视图恢复为1，即显示光标。如果用户成功选择了一个对象，则关闭高亮并返回对象句柄。如果没有选择对象，则返回NULL_TAG。
>
> 这段代码的目的是让用户能够从NX图形窗口中选择一个绘图曲线，并获取该曲线的对象句柄。这是进行NX二次开发时经常需要实现的功能之一。
>
