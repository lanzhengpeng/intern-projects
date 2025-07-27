### 【2688】select planes 选择平面

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_planes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_plane_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_planes(char *prompt, tag_t **planes)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select planes", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_planes, NULL, &resp, &cnt, planes));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*planes)[ii], FALSE));  
        return cnt;  
    }

```

#### 代码解析

> 这段代码是用于在NX CAD软件中实现一个选择平面的对话框的二次开发代码。
>
> 代码的主要功能包括：
>
> 1. 定义了一个静态函数mask_for_planes，用于设置选择过滤条件，只选择平面类型的实体。
> 2. 定义了一个静态函数select_planes，用于打开选择对话框，提示用户选择平面，并获取选择结果。
> 3. 在select_planes函数中，使用UF_UI_select_with_class_dialog函数打开选择对话框，并设置选择过滤条件为mask_for_planes函数。
> 4. 用户在对话框中选择平面后，函数获取选择结果，并关闭对话框。
> 5. 遍历选择结果，关闭每个平面的高亮显示。
> 6. 返回选择平面的数量。
>
> 总的来说，这段代码实现了在NX CAD软件中，打开一个对话框让用户选择平面，并获取选择结果的功能，为后续的平面处理提供了基础。
>
