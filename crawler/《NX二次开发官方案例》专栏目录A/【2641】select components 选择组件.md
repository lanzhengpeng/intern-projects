### 【2641】select components 选择组件

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_components(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_component_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_components(char *prompt, tag_t **components)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select components", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_components, NULL, &resp,  
            &cnt, components));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*components)[ii], FALSE));  
        return cnt;  
    }

```

#### 代码解析

> 这是段NX二次开发代码，主要功能是使用UF_UI接口在NX中选择组件。
>
> 主要步骤包括：
>
> 1. 定义了一个mask_for_components函数，用于设置选择组件的类型为UF_component_type。
> 2. 定义了一个select_components函数，用于弹出选择组件对话框，并根据mask_for_components设置的mask来选择组件。用户可以在对话框中输入提示文字，选择范围为整个装配，最后返回选择的组件数量。
> 3. 在select_components函数中，先清空原有的选择，然后启用mask_for_components设置的mask。接着弹出对话框让用户选择组件。最后，将选中的组件的高亮去掉，并返回组件数量。
> 4. 这段代码通过UF_UI接口实现了在NX中选择组件的功能，包括设置选择范围、设置类型、弹窗选择等操作。通过二次开发，可以方便地在NX中实现自定义的选择组件功能。
>
