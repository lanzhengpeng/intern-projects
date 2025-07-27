### 【2583】select a linear edge 选择一条线性边

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_linear_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_LINEAR_EDGE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_linear_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select linear edge", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_linear_edges, NULL, &resp,  
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

> 这段代码是用于NX二次开发，其主要功能是实现线性边的选择。
>
> 首先，代码定义了一个静态函数mask_for_linear_edges，用于设置选择掩码，以便只选择线性边。
>
> 其次，代码定义了一个静态函数select_a_linear_edge，用于实现线性边的交互式选择。该函数通过调用UF_UI_select_with_single_dialog函数，弹出一个对话框，提示用户选择线性边。在选择后，函数会高亮显示所选边，并返回边的tag。
>
> 这段代码的主要作用是实现NX中线性边的交互式选择，并返回选择的边的tag。通过设置选择掩码，可以确保只选择线性边。
>
