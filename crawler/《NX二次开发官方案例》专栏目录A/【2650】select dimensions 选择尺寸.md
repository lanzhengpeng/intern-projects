### 【2650】select dimensions 选择尺寸

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_dimensions(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_dimension_type, UF_all_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_dimensions(char *prompt, tag_t **dimensions)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select dimensions", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_dimensions, NULL, &resp, &cnt,   
            dimensions));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*dimensions)[ii], FALSE));  
        return cnt;  
    }

```

#### 代码解析

> 根据代码的注释和上下文，这段代码是用于在NX（NX是Siemens PLM Software的CAD/CAM/CAE软件）中进行二次开发，以实现选择并高亮显示特征线的功能。
>
> 具体来说，代码实现了以下功能：
>
> 1. 定义了一个mask_for_dimensions函数，用于设置选择过滤器，使得在选择对话框中只能选择特征线。
> 2. 定义了一个select_dimensions函数，用于显示选择对话框，提示用户选择特征线。在选择完成后，遍历用户选择的所有特征线，并取消高亮显示。
> 3. select_dimensions函数首先设置选择过滤器，然后调用NX提供的UF_UI_select_with_class_dialog函数显示选择对话框，并获取用户的选择结果。最后，取消高亮显示用户选择的所有特征线。
> 4. 这段代码的主要目的是实现一个简单的特征线选择功能，用户可以通过选择对话框选择特征线，选择完成后，特征线会自动取消高亮显示。
>
> 综上所述，这段代码为NX的二次开发提供了一种便捷的特征线选择功能，用户可以通过简单的交互来选择特征线，而无需复杂的操作。
>
