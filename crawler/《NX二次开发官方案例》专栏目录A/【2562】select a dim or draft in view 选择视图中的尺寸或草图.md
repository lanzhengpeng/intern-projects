### 【2562】select a dim or draft in view 选择视图中的尺寸或草图

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_dim_or_draft(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_drafting_entity_type, UF_all_subtype, 0 },  
                        { UF_dimension_type, UF_all_subtype, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_dim_or_draft(char *prompt, tag_t *view)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog("Select dimension or drafting aid",  
            prompt, UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_dim_or_draft, NULL,  
            &resp, &object, cp, view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是用于NX CAD系统进行二次开发，用于选择草绘实体或尺寸标注。以下是主要功能的介绍：
>
> 1. mask_for_dim_or_draft函数：该函数用于设置选择过滤器，只允许选择草绘实体或尺寸标注。它设置了两个选择掩码，一个用于草绘实体，另一个用于尺寸标注，并调用UF_UI_set_sel_mask函数来启用这些掩码。
> 2. select_a_dim_or_draft函数：该函数用于打开一个选择对话框，提示用户选择一个草绘实体或尺寸标注。它设置选择范围为任意装配中的对象，并使用之前定义的选择掩码。然后，它调用UF_UI_select_with_single_dialog函数来显示对话框并获取用户的选择。如果用户选择了对象，则返回该对象的tag。如果没有选择，则返回NULL_TAG。
> 3. 代码中使用了UF_CALL宏来调用NX的API函数，并在调用失败时进行错误处理。
> 4. UF_DISP_set_highlight函数用于取消高亮显示选中对象。
>
> 综上所述，这段代码实现了在NX CAD中打开选择对话框，让用户选择草绘实体或尺寸标注，并返回所选对象的tag的功能。它通过设置选择过滤器来限定可选择的对象类型。
>
