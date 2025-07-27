### 【2625】select an ordinate dimension 选择一个坐标尺寸

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_ordinate_dimensions(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_dimension_type, UF_dim_ordinate_horiz_subtype, 0 },  
                        { UF_dimension_type, UF_dim_ordinate_vert_subtype, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_an_ordinate_dimension(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a dimension", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_ordinate_dimensions, NULL,  
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

> 这是段NX二次开发代码，主要功能是选择一个坐标尺寸。
>
> 主要步骤包括：
>
> 1. 定义了一个静态函数mask_for_ordinate_dimensions，用于设置选择过滤器，仅允许选择水平或垂直的坐标尺寸。
> 2. 定义了另一个静态函数select_an_ordinate_dimension，用于使用单选对话框让用户选择一个尺寸对象。
> 3. 在select_an_ordinate_dimension函数中，使用UF_UI_select_with_single_dialog函数弹出单选对话框，允许用户选择任何组件中的尺寸对象。
> 4. 对话框的过滤条件由mask_for_ordinate_dimensions函数提供，确保只能选择坐标尺寸。
> 5. 选择完成后，获取选择的尺寸对象标签，并关闭高亮显示。
> 6. 如果用户选择了尺寸对象，则返回该对象的标签；如果没有选择，则返回NULL_TAG。
>
> 通过这些步骤，代码实现了让用户选择一个坐标尺寸的功能，可以用于后续的尺寸处理。
>
