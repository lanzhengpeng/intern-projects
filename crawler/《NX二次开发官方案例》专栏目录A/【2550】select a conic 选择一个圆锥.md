### 【2550】select a conic 选择一个圆锥

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_conics(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_circle_type, 0, 0 },  
                        { UF_conic_type, UF_all_subtype, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_CIRCULAR_EDGE },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ELLIPTICAL_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    extern tag_t select_a_conic(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a conic", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_conics, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发，用于选择一个圆锥曲线（例如圆、椭圆等）的示例代码。
>
> 代码主要包含以下部分：
>
> 1. mask_for_conics函数：用于设置选择掩码，以允许选择圆锥曲线类型。
> 2. select_a_conic函数：用于弹出一个对话框，提示用户选择一个圆锥曲线。其中，mask_for_conics函数用于设置选择掩码，只允许选择圆锥曲线类型。
> 3. 在select_a_conic函数中，通过UF_UI_select_with_single_dialog函数弹出选择对话框，并设置适当的参数。用户选择对象后，函数返回选择的标签。
> 4. 如果用户选择了对象，则返回对象标签。否则，返回NULL_TAG。
> 5. 整个代码流程清晰，注释明确，是一个很好的二次开发示例代码。
>
