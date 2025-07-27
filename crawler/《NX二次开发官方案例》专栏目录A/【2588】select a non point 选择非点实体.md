### 【2588】select a non point 选择非点实体

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_no_points(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_point_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_ALL_AND_DISABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_non_point(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an object (not a point)",  
            prompt, UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_no_points, NULL,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 这段NX Open C++代码实现了选择非点对象的功能。其主要功能如下：
>
> 1. mask_for_no_points函数：用于设置选择过滤器，以排除点对象，仅允许选择非点对象。
> 2. select_a_non_point函数：使用单选对话框让用户选择一个非点对象，并返回所选对象的tag。在选择前，它会调用mask_for_no_points来设置过滤器。
> 3. 在选择完成后，使用UF_DISP_set_highlight函数取消高亮显示。
> 4. 返回所选对象的tag，如果用户取消选择则返回NULL_TAG。
>
> 通过这段代码，开发者可以在NX中方便地选择非点对象，并获取其tag，以便进行后续操作。
>
