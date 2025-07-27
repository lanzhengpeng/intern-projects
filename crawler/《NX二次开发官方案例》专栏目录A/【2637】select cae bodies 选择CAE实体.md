### 【2637】select cae bodies 选择CAE实体

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_cae_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_caegeom_type, UF_caegeom_body_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_cae_bodies(char *prompt, tag_t **cae_bodies)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select Polygon Bodies", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_cae_bodies, NULL, &resp, &cnt, cae_bodies));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*cae_bodies)[ii], FALSE));  
        return cnt;  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，用于选择和获取NX模型中的实体。
>
> 主要功能包括：
>
> 1. 定义了一个函数mask_for_cae_bodies，用于设置选择过滤器，只允许选择实体类型的几何体。
> 2. 定义了一个函数select_cae_bodies，用于打开选择对话框，让用户选择实体，并获取用户选择的实体数组。
> 3. mask_for_cae_bodies函数中，首先创建一个选择过滤器，只允许选择实体类型的几何体。然后调用UF_UI_set_sel_mask函数，设置选择模式为清除已有选择并启用特定过滤器。
> 4. select_cae_bodies函数中，打开选择对话框，设置提示文字、选择范围、过滤器等，让用户选择实体。然后获取用户选择的实体数量和数组。最后关闭高亮显示。
> 5. 整体来看，这段代码提供了选择和获取NX模型中实体的功能，并具有友好性提示和过滤选择。
>
