### 【2584】select a linear object 选择线性对象

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_linear_objects(UF_UI_selection_p_t select,  
        void *type)  
    {  
        UF_UI_mask_t  
            mask[3] = { { UF_line_type, 0, 0},  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_LINEAR_EDGE },  
                        { UF_datum_axis_type, 0, 0 }};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 3, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_linear_object(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select edge/datum axis", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_linear_objects,  
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

> 这段代码是用于在NX中实现线性对象选择的功能。具体来说，代码定义了两个函数：
>
> 1. mask_for_linear_objects：该函数用于设置选择掩码，以允许用户仅选择线、实体边或基准轴。它接收选择参数和类型参数，并返回一个掩码数组，其中包含允许选择的线性对象类型。
> 2. select_a_linear_object：该函数用于提示用户选择一个线性对象，如边或基准轴。它接收提示字符串，并使用UF_UI_select_with_single_dialog函数显示选择对话框，使用mask_for_linear_objects函数设置选择掩码。用户选择后，函数会获取选择的标签并高亮显示。若用户成功选择了对象，则返回对象的标签，否则返回NULL_TAG。
>
> 总体而言，这段代码提供了在NX中实现线性对象选择的功能，通过设置选择掩码来限制用户的选择范围，并返回用户选择的线性对象的标签。
>
