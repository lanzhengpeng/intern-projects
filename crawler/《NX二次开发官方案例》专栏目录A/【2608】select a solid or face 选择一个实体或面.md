### 【2608】select a solid or face 选择一个实体或面

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_solids_and_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE } } ;  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_solid_or_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_solids_and_faces, NULL, &resp,  
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

> 这段NX Open C++代码实现了以下功能：
>
> 1. 定义了一个名为mask_for_solids_and_faces的静态函数，用于设置选择集选择实体和面的掩码。
> 2. 定义了一个名为select_a_solid_or_face的静态函数，用于打开一个单选对话框，让用户选择一个实体或面。该函数返回用户选择的实体或面的tag。
> 3. 在select_a_solid_or_face函数中，调用UF_UI_select_with_single_dialog函数打开单选对话框，并设置选择范围、选择掩码和提示信息。
> 4. UF_UI_select_with_single_dialog函数返回用户选择结果，若用户成功选择了一个实体或面，则返回UF_UI_OBJECT_SELECTED或UF_UI_OBJECT_SELECTED_BY_NAME，此时取消高亮并返回所选对象tag。
> 5. 若用户未选择或选择失败，则返回NULL_TAG。
> 6. 这段代码通过二次开发，实现了在NX中打开单选对话框，让用户选择实体或面，并获取用户选择结果的功能。可以用于NX Open的二次开发中。
>
