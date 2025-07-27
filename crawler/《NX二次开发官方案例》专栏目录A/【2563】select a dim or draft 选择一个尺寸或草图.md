### 【2563】select a dim or draft 选择一个尺寸或草图

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_dim_or_draft(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_drafting_entity_type, UF_all_subtype, 0 },  
                        { UF_dimension_type, UF_all_subtype, 0 },  
                        { UF_tabular_note_type, UF_all_subtype, 0 },  
                        { UF_tol_feature_instance_type, 0, 0} };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_dim_or_draft(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select dimension or drafting aid",  
            prompt, UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_dim_or_draft, NULL,  
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

> 这段代码是用于NX二次开发的选择标注或绘图辅助元素的功能。
>
> 主要功能包括：
>
> 1. 定义一个选择掩码，用于限制选择范围为标注、绘图辅助元素、表格注释和公差特征实例。
> 2. 定义一个选择函数select_a_dim_or_draft，用于在对话框中提示用户选择标注或绘图辅助元素。
> 3. 在选择函数中，使用UF_UI_select_with_single_dialog函数弹出对话框，让用户选择。在选择掩码中只允许标注、绘图辅助等类型。
> 4. 如果用户选择了一个对象，则取消高亮，并返回对象的tag。
> 5. 如果用户没有选择任何对象，则返回NULL_TAG。
> 6. 在主函数中，调用select_a_dim_or_draft函数，获取用户选择的标注或绘图辅助元素，并进行后续操作。
> 7. 使用UF_DISP_set_highlight函数来取消选择对象的高亮显示。
>
> 综上所述，这段代码的主要功能是实现了在NX中通过对话框选择标注或绘图辅助元素，并返回其tag，以便进行后续处理。
>
