### 【2696】select points 选择点

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_points(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_point_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_points(char *prompt, tag_t **points)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select points", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_points, NULL, &resp, &cnt, points));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*points)[ii], FALSE));  
        return cnt;  
    }

```

#### 代码解析

> 这段代码是用于在NX中进行点选择并进行后续操作的基础二次开发代码。
>
> 具体来说，代码的主要功能包括：
>
> 1. 定义了mask_for_points函数，用于设置选择过滤条件，只允许选择点。
> 2. 定义了select_points函数，用于弹出选择对话框，提示用户选择点，并获取选择的点数组。
> 3. 在select_points函数中，使用UF_UI_select_with_class_dialog函数来弹出选择对话框，用户可以输入提示文字进行选择。选择范围设置为工作部件，过滤条件设置为只允许选择点。
> 4. 选择结束后，使用UF_DISP_set_highlight函数将选择的点取消高亮显示。
> 5. 最终，select_points函数返回选择的点数量和点数组，供后续代码进行其他操作。
>
> 这段代码为后续的点选择、操作等提供了基础支持，是一个典型的NX二次开发点选择模块。
>
