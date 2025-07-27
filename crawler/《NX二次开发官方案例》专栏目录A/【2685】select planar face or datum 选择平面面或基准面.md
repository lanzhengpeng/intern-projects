### 【2685】select planar face or datum 选择平面面或基准面

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_planar_face_datum(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_PLANAR_FACE },  
                        { UF_datum_plane_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_planar_face_or_datum(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select planar face/datum", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART,  
            mask_for_planar_face_datum, NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        return NULL_TAG;  
    }

```

```cpp
tag_t face_or_datum = select_planar_face_or_datum("请选择平面面或基准面");
if(face_or_datum) {
    // 处理选中的平面面或基准面
}

```

#### 代码解析

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 自定义选择掩码：定义了一个静态函数mask_for_planar_face_datum，用于在对话框中选择平面面或基准面。该函数设置了选择掩码，使得用户只能选择平面面或基准面。
> 2. 选择平面面或基准面：定义了一个静态函数select_planar_face_or_datum，用于弹出一个对话框，让用户选择平面面或基准面。函数设置了自定义选择掩码，并获取用户的选择结果。
> 3. 使用示例：
>
> ```cpp
> tag_t face_or_datum = select_planar_face_or_datum("请选择平面面或基准面");
> if(face_or_datum) {
>     // 处理选中的平面面或基准面
> }
> ```
>
> 1. 功能总结：这段代码通过自定义选择掩码，实现了在对话框中让用户选择平面面或基准面的功能。选择完成后，返回选中的对象标签，以便后续处理。
>
