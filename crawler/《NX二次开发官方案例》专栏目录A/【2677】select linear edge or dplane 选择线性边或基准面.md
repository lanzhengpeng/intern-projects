### 【2677】select linear edge or dplane 选择线性边或基准面

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_linear_edges_or_dplanes(UF_UI_selection_p_t select,   
        void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { UF_solid_type, 0, UF_UI_SEL_FEATURE_LINEAR_EDGE },  
                      { UF_datum_plane_type, 0, 0 }};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_linear_edge_or_dplane(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select edge/datum plane", prompt,   
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_linear_edges_or_dplanes,   
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

> 这段NX Open C++代码用于实现以下功能：
>
> 1. 定义了一个静态函数mask_for_linear_edges_or_dplanes，用于设置选择集的过滤条件，仅允许选择实体边或基准平面。
> 2. 定义了一个静态函数select_a_linear_edge_or_dplane，用于显示一个选择对话框，提示用户选择一个实体边或基准平面，并返回选中的对象标签。
> 3. 在select_a_linear_edge_or_dplane函数中，使用UF_UI_select_with_single_dialog函数显示选择对话框，并设置过滤条件。用户选择对象后，返回选中对象的标签。
> 4. 如果用户成功选择对象，关闭高亮显示，并返回选中对象的标签。
> 5. 如果用户没有选择对象，返回NULL_TAG。
> 6. 这段代码通过设置选择集的过滤条件，仅允许选择实体边或基准平面，简化了用户的选择操作，提高了用户体验。
> 7. 使用NX Open C++ API实现了NX的选择功能，展示了NX Open的二次开发能力。
>
> 这段代码的作用是提供一个选择对话框，让用户仅能选择实体边或基准平面，返回选中对象的标签。通过设置选择集过滤条件，简化了用户的选择操作。
>
