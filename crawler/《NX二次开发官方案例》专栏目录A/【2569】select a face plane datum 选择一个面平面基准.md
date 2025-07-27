### 【2569】select a face plane datum 选择一个面平面基准

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_face_plane_datums(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[3] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                        { UF_plane_type, 0, 0 },  
                        { UF_datum_plane_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 3, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_a_face_plane_datum(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_face_plane_datums, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX二次开发中用于选择面、平面或基准面的功能。
>
> 首先，定义了一个选择掩码数组mask，用于在对话框中限制选择类型。掩码数组中定义了三种类型：实体类型、平面类型和基准面类型，并设置了选择模式为任意面。
>
> 然后，定义了一个函数select_a_face_plane_datum，用于弹出一个对话框，让用户选择一个面、平面或基准面。在选择掩码数组mask的过滤下，用户只能选择这三种类型的对象。
>
> 在选择完成后，函数返回选中的对象tag。如果没有选择，则返回NULL_TAG。
>
> 这个函数可以方便地在二次开发中让用户选择一个面、平面或基准面，然后基于这个选择进行后续操作。掩码数组的使用有效地过滤了选择范围，提高了交互的效率。
>
