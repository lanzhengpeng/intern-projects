### 【2698】select reference object 选择参考对象

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_reference_object(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_PLANAR_FACE },  
                        { UF_datum_plane_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_LINEAR_EDGE },  
                        { UF_datum_axis_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_reference_object(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select object", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART,  
            mask_for_reference_object, NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX二次开发中的选择参考对象的部分，主要功能如下：
>
> 1. 静态函数mask_for_reference_object：定义了一个函数，用于设置选择掩码，允许选择平面面、线性边、基准面和基准轴等特征。
> 2. 静态函数select_reference_object：定义了一个函数，用于选择参考对象。函数通过设置选择掩码，打开选择对话框，提示用户选择对象，并返回所选对象的标签。
> 3. 代码流程：首先定义了允许选择的特征类型，然后调用UF_UI_select_with_single_dialog函数打开选择对话框，用户选择对象后，关闭对话框，并返回所选对象的标签。
> 4. 提示：在函数内部，设置选择掩码允许用户选择面、边、基准面和基准轴等特征。在用户选择对象后，关闭高亮显示并返回对象标签。
> 5. 返回值：函数返回所选对象的标签，如果没有选择对象，则返回NULL_TAG。
> 6. 应用：这段代码可以用于NX二次开发，实现选择参考对象的功能，如选择平面面、线性边、基准面和基准轴等，以便后续进行几何计算或其他操作。
>
