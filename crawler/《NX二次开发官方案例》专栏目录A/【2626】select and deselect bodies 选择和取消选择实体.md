### 【2626】select and deselect bodies 选择和取消选择实体

#### 代码

```cpp
    /* 里海 */  
    static int preselect_and_mask_for_bodies(UF_UI_selection_p_t select,  
        void *user_data)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        preselect_data_p_t  
            preselects;  
        preselects = (preselect_data_p_t) user_data;  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_add_to_sel_list(select, preselects->n_items,  
                preselects->items, TRUE)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_and_deselect_bodies(char *prompt, tag_t *preselects,  
        int n_preselects, tag_t **selections)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        preselect_data_t  
            presets;  
        presets.n_items = n_preselects;  
        presets.items = preselects;  
        UF_CALL(UF_UI_select_with_class_dialog("Select/Deselect bodies", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, preselect_and_mask_for_bodies,  
            (void *)&presets, &resp, &cnt, selections));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*selections)[ii], FALSE));  
        return cnt;  
    }

```

#### 代码解析

> 这段代码是NX二次开发中的用户界面选择相关代码，主要用于实现特定类型实体的选择和反选功能。
>
> 代码的核心逻辑包括：
>
> 1. 定义一个预选择函数preselect_and_mask_for_bodies，用于设置选择掩码，仅允许选择实体特征。同时，将传入的预选实体添加到选择列表中。
> 2. 定义一个选择函数select_and_deselect_bodies，它接受提示信息、预选实体数组和预选实体数量，以及一个传出选择实体数组。
> 3. 在选择函数中，创建一个预选择数据结构，并设置预选实体。
> 4. 调用NX的UF_UI_select_with_class_dialog函数，弹出选择对话框，并传入预选择函数。对话框中会显示传入的提示信息，用户可以在其中选择或反选实体。
> 5. 选择完成后，获取用户的选择结果，存放在传出数组中，并关闭对话框。
> 6. 遍历传出数组，关闭每个选择实体的高亮显示。
> 7. 返回选择实体的数量。
>
> 通过这段代码，用户可以方便地选择和反选NX模型中的实体特征，为后续的建模操作提供了便利。代码结构清晰，逻辑严谨，具有良好的可读性和可维护性。
>
