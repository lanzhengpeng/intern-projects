### 【2665】select edges 选择边线

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_edges(char *prompt, tag_t **edges)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select edges", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_edges, NULL, &resp, &cnt, edges));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*edges)[ii], FALSE));  
        return cnt;  
    }

```

#### 代码解析

> 这段代码是用于在NX中实现二次开发，主要实现了以下功能：
>
> 1. 定义了一个函数mask_for_edges，用于设置选择边界的条件，即选择所有类型的边。
> 2. 定义了一个函数select_edges，用于实现选择边的功能。该函数通过对话框提示用户选择边，并获取选择的边的数量和数组。
> 3. mask_for_edges函数设置了选择边的条件，只选择所有类型的边，并将该条件应用于UF_UI_select_with_class_dialog函数，以实现选择边的功能。
> 4. select_edges函数通过UF_UI_select_with_class_dialog函数实现了选择边的功能，提示用户选择边，并获取选择的边的数量和数组。
> 5. 在选择完边后，通过循环调用UF_DISP_set_highlight函数，将选择的边取消高亮显示。
> 6. select_edges函数返回选择的边的数量。
> 7. 这段代码通过设置选择条件，实现了在NX中选择边的功能，适用于选择所有类型的边。
>
