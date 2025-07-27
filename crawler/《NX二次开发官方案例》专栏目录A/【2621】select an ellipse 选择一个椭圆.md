### 【2621】select an ellipse 选择一个椭圆

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_ellipses(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_conic_type, UF_conic_ellipse_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    extern tag_t select_an_ellipse(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an Ellipse", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_ellipses, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是一个NX二次开发示例，用于选择椭圆体。
>
> 1. mask_for_ellipses函数设置选择过滤条件，只允许选择椭圆体。
> 2. select_an_ellipse函数打开一个选择对话框，提示用户选择一个椭圆体。
> 3. 使用UF_UI_select_with_single_dialog函数来实现选择功能，传入选择过滤条件和提示信息。
> 4. 选择完成后，取消高亮显示所选椭圆体，并返回其tag。
> 5. 如果用户取消选择，则返回NULL_TAG。
>
> 总的来说，这段代码实现了在NX中打开对话框并选择椭圆体的功能。通过设置选择过滤条件，可以确保只选择椭圆体。
>
