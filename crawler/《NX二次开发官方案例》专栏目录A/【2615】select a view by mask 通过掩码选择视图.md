### 【2615】select a view by mask 通过掩码选择视图

#### 代码

```cpp
    /* There are two overloads select_a_view_by_mask included in this file. *  
     * One returns the tag of the view and the other returns the view name. *  
     * Be sure to delete the one you don't want to include in your program. */  
    static int mask_for_views(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_view_type, UF_all_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_view_by_mask(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_views, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical select_a_view_by_mask(char *prompt, char *view_name)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view_tag;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_views, NULL, &resp,  
            &object, cp, &view_tag));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            UF_CALL(UF_OBJ_ask_name(view_tag, view_name));  
            return true;  
        }  
        else return false;  
    }

```

#### 代码解析

> 这段代码是NX Open的二次开发代码，主要功能是选择一个视图。
>
> 具体来说，代码中包含了两个函数：
>
> 1. select_a_view_by_mask()：这个函数接受一个提示字符串作为参数，然后弹出选择对话框，允许用户在装配中选择一个视图。函数内部先调用mask_for_views()设置选择掩码，只允许选择视图类型。然后调用UF_UI_select_with_single_dialog()弹出选择对话框。用户选择后，函数返回视图的tag。
> 2. select_a_view_by_mask()：这个函数接受提示字符串和视图名称字符串作为参数。与上一个函数类似，它也会弹出选择对话框，但返回的是视图的名称，而不是tag。
>
> 这两个函数通过重载实现了不同的返回值，一个是返回视图的tag，另一个是返回视图的名称。开发者可以根据需要选择使用其中一个函数。
>
