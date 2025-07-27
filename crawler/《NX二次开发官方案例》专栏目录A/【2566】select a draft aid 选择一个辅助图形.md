### 【2566】select a draft aid 选择一个辅助图形

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_draft_aids(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_drafting_entity_type, UF_all_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_draft_aid(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a drafting aid", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_draft_aids, NULL, &resp,  
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

> 这段代码是用于在NX中实现选择绘图辅助线的二次开发代码，其主要功能包括：
>
> 1. 定义了mask_for_draft_aids函数，用于设置选择过滤条件，只允许选择绘图辅助线。
> 2. 定义了select_a_draft_aid函数，用于弹出选择对话框，让用户选择一个绘图辅助线。
> 3. mask_for_draft_aids函数中，设置了选择过滤条件，只允许选择绘图辅助线实体。
> 4. select_a_draft_aid函数中，使用了UF_UI_select_with_single_dialog函数弹出选择对话框，并调用mask_for_draft_aids函数设置选择过滤条件。
> 5. 用户选择后，获取选择结果，如果选择了绘图辅助线，则取消高亮显示，并返回该绘图辅助线的tag。
> 6. 如果没有选择绘图辅助线，则返回NULL_TAG。
> 7. 这段代码实现了在NX中选择绘图辅助线的功能，可以用于后续的二次开发处理。
>
