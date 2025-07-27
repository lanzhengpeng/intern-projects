### 【2635】select bodies 选择实体

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_bodies(char *prompt, tag_t **bodies)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select solids or sheets", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp, &cnt,  
            bodies));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*bodies)[ii], FALSE));  
        return cnt;  
    }

```

#### 代码解析

> 这段代码是用于在NX CAD软件中进行二次开发，主要实现了以下功能：
>
> 1. 自定义选择器掩码：mask_for_bodies函数定义了一个选择器掩码，用于在交互式选择对话框中启用特定类型的选择，即仅允许选择实体特征体。
> 2. 选择体：select_bodies函数用于通过交互式选择对话框选择实体或片体。该函数接收提示信息，然后调用NX的UI选择函数，根据掩码进行选择，最后将选中的体存储在数组中，并关闭高亮显示。
> 3. 代码功能：总体来说，这段代码实现了在NX中自定义选择器掩码，并通过对话框让用户选择所需的实体或片体，为后续的二次开发功能提供基础。
> 4. 使用方法：开发者可以通过调用select_bodies函数，传入提示信息，并获取选中的体数组，然后基于这些体进行后续的二次开发操作。掩码定义和选择逻辑已经封装在函数中，方便开发者使用。
> 5. NX二次开发：这段代码展示了如何使用NX提供的UI选择函数进行二次开发，为用户提供了交互式选择实体的功能，是NX二次开发中的一个常见功能。
>
