### 【2548】select a circular edge 选择一个圆形边

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_circular_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_CIRCULAR_EDGE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_circular_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select circular edge", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_circular_edges, NULL, &resp,  
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

> 这段代码是NX Open的二次开发示例，主要用于在NX中实现选择圆边功能。
>
> 主要步骤包括：
>
> 1. 定义一个函数mask_for_circular_edges，用于设置选择过滤器，仅允许选择圆形边。
> 2. 定义一个函数select_a_circular_edge，用于打开选择对话框，提示用户选择圆形边。
> 3. 在select_a_circular_edge函数中，使用UF_UI_select_with_single_dialog函数打开单选对话框，设置选择过滤器和提示信息。
> 4. 用户在对话框中选择圆形边后，函数返回圆形边的tag。如果没有选择，则返回空tag。
> 5. 使用UF_DISP_set_highlight函数关闭高亮显示。
>
> 通过这两个函数，可以方便地在NX中实现选择圆形边的功能，用户只需调用select_a_circular_edge函数即可。这段代码体现了NX Open二次开发的基本流程，即先定义选择过滤器，然后调用选择函数，最后处理选择结果。
>
