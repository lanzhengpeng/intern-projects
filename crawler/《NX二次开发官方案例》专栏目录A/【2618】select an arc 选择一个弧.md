### 【2618】select an arc 选择一个弧

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_arcs(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_circle_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_CIRCULAR_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_an_arc(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an arc", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_arcs, NULL, &resp,  
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

> 这是段NX二次开发的代码，主要实现了选择圆弧的功能。
>
> 1. mask_for_arcs函数：该函数用于设置选择过滤器，只允许选择圆形边线。
> 2. select_an_arc函数：该函数用于选择一个圆弧，并返回其tag。
> 3. 在select_an_arc函数中，首先使用UF_UI_select_with_single_dialog函数弹出一个选择对话框，让用户选择一个圆弧。
> 4. 根据用户的选择，函数返回选中的圆弧的tag，或者如果没有选择，则返回NULL_TAG。
> 5. 代码中使用了UF_DISP_set_highlight函数来取消选中圆弧的高亮显示。
> 6. 整体来看，这段代码实现了在NX中弹出对话框，让用户选择一个圆弧，并返回圆弧的tag的功能，为后续的圆弧处理提供了支持。
>
