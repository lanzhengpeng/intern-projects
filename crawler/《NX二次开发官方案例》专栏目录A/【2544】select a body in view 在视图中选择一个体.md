### 【2544】select a body in view 在视图中选择一个体

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
    static tag_t select_a_body_in_view(char *prompt, tag_t *view)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog("Select a body", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp,  
            &object, cp, view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发中实现视图内选择一个体的功能。具体来说，它包含两个函数：
>
> 1. mask_for_bodies函数：设置选择过滤器，只允许选择实体类型。
> 2. select_a_body_in_view函数：显示一个选择对话框，提示用户在指定的视图内选择一个实体。这个函数首先清除并设置选择过滤器，然后显示选择对话框，提示用户选择。在选择完成后，如果选择成功，则取消高亮并返回选择实体的tag。如果选择失败，则返回NULL_TAG。
>
> 这段代码通过二次开发的方式，实现了在指定视图内选择一个实体的功能，为后续操作提供了实体对象。
>
