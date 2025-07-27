### 【2602】select a sheet edge 选择一个板边

#### 代码

```cpp
    /* 里海 */  
    static int filter_body_type(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        int  
            body_type;  
        tag_t  
            body;  
        UF_CALL(UF_MODL_ask_edge_body(object, &body));  
        UF_CALL(UF_MODL_ask_body_type(body, &body_type));  
        if (body_type == UF_MODL_SHEET_BODY)  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    /* 里海 */  
    static int mask_for_sheet_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_body_type, NULL, type)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_sheet_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an edge", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_sheet_edges, NULL, &resp,  
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

> 这段NX二次开发代码实现了以下功能：
>
> 1. 过滤选择器：定义了一个函数filter_body_type，用于过滤选择器，只允许选择面体(sheet body)的边。
> 2. 选择器掩码：定义了一个函数mask_for_sheet_edges，设置选择器掩码，仅选择实体边。
> 3. 选择面体边：定义了一个函数select_a_sheet_edge，使用对话框提示用户选择一个边，并应用之前定义的过滤和掩码，以确保只选择面体的边。
> 4. 返回选择对象：如果用户选择了一个边，则取消高亮并返回该边的tag，否则返回NULL_TAG。
> 5. 用户交互：通过对话框提示用户选择，实现了用户交互。
> 6. 选择过滤：通过设置选择器掩码和过滤函数，实现了只选择特定类型的边。
> 7. 取消高亮：在选择后取消高亮，避免影响后续操作。
> 8. 返回结果：最终返回用户选择的边的tag。
>
> 综上所述，这段代码通过自定义选择器掩码和过滤函数，实现了在NX中仅选择面体边的功能，并通过对话框提示用户进行选择。
>
