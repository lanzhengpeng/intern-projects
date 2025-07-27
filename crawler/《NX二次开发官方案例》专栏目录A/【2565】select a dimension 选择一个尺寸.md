### 【2565】select a dimension 选择一个尺寸

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
    static tag_t select_a_dimension(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a dimension", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_dimensions, NULL, &resp,  
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

> 这段代码是NX二次开发中的选择维度功能实现。具体来说，它定义了两个静态函数：
>
> 1. mask_for_dimensions函数：设置选择集的选择掩码，只允许选择维度特征，包括所有子类型。
> 2. select_a_dimension函数：弹出一个选择对话框，提示用户选择一个维度。在选择时，会调用mask_for_dimensions函数来设置选择掩码。用户选择后，获取选择的维度对象标签并返回。如果没有选择，则返回NULL标签。
>
> 这段代码通过自定义选择掩码和选择对话框，实现了NX中选择特定类型特征（这里是维度）的功能，为后续的二次开发操作（如修改或删除维度）提供了基础。选择完成后，代码还会关闭高亮显示。
>
