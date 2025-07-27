### 【2627】select and deselect faces 选择和取消选择面

#### 代码

```cpp
    struct preselect_data_s {  
        int     n_items;  
        tag_t   *items;  
    };  
    typedef struct preselect_data_s preselect_data_t, *preselect_data_p_t;  
    /* 里海 */  
    static int preselect_and_mask_for_faces(UF_UI_selection_p_t select,  
        void *user_data)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
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
    static int select_and_deselect_faces(char *prompt, tag_t *preselects,  
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
        UF_CALL(UF_UI_select_with_class_dialog("Select/Deselect faces", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, preselect_and_mask_for_faces,  
            (void *)&presets, &resp, &cnt, selections));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*selections)[ii], FALSE));  
        return cnt;  
    }

```

#### 代码解析

> 这段代码是用于NX CAD软件的二次开发，其主要功能是实现预选面然后进行选择或取消选择的操作。
>
> 具体来说，这段代码包含以下几个关键点：
>
> 1. 定义了一个结构体preselect_data_t，用于存储预选面的信息，包括预选面的数量和面本身。
> 2. 定义了一个函数preselect_and_mask_for_faces，用于设置选择掩码，只允许选择面，并添加预选面到选择列表中。
> 3. 定义了一个函数select_and_deselect_faces，用于实现选择或取消选择面的功能。首先调用NX的选择对话框，传入预选面信息和选择掩码。然后遍历返回的选择结果，取消高亮显示。
> 4. 通过调用UF_DISP_set_highlight取消高亮显示，避免选择的元素被高亮显示。
> 5. 返回选择结果的数量。
> 6. 使用UF_UI_select_with_class_dialog实现选择对话框。
>
> 总的来说，这段代码实现了在NX中通过预选面进行选择或取消选择面的功能，通过设置选择掩码限制只选择面，并通过取消高亮显示来避免选择结果被高亮显示。
>
