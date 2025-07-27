### 【2611】select a tabular note 选择一个表格注释

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_tabular_notes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_tabular_note_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_tabular_note(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a tabular note", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_tabular_notes, NULL, &resp,  
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

> 这段代码是用于NX二次开发，主要实现以下功能：
>
> 1. 定义了一个静态函数mask_for_tabular_notes，用于设置选择过滤条件，只选择表格注释。
> 2. 定义了一个静态函数select_a_tabular_note，用于弹出选择对话框，让用户选择一个表格注释。其中：弹出对话框的标题为"Select a tabular note"提示信息为传入的prompt参数选择范围设置为工作部件使用mask_for_tabular_notes函数设置选择过滤条件用户选择后，获取选择的对象标识、视图、位置等信息
> 3. 弹出对话框的标题为"Select a tabular note"
> 4. 提示信息为传入的prompt参数
> 5. 选择范围设置为工作部件
> 6. 使用mask_for_tabular_notes函数设置选择过滤条件
> 7. 用户选择后，获取选择的对象标识、视图、位置等信息
> 8. select_a_tabular_note函数返回用户选择的表格注释的对象标识，如果没有选择则返回NULL_TAG。
> 9. 使用UF_DISP_set_highlight函数取消选择的表格注释的高亮显示。
>
> 这段代码的主要作用是让用户能够选择一个NX模型中的表格注释，获取其对象标识，以便后续进行操作。
>
