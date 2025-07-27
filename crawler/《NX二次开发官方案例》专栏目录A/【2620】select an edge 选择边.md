### 【2620】select an edge 选择边

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_an_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an edge", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_edges, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是用于在NX中进行二次开发的示例代码，其功能是选择一个边并返回该边的标签。具体来说：
>
> 1. mask_for_edges函数用于设置选择集，只允许选择特征中的任意边。它设置了选择集的掩码，只允许选择边。
> 2. select_an_edge函数用于通过单选对话框让用户选择一个边。它首先调用UF_UI_select_with_single_dialog函数，让用户在对话框中选择一个边。如果用户选择了边，则返回该边的标签；如果没有选择，则返回NULL_TAG。
> 3. 在select_an_edge函数中，使用mask_for_edges函数来设置选择集的掩码，确保只允许选择边。
> 4. select_an_edge函数中，如果用户选择了边，则调用UF_DISP_set_highlight函数取消边的高亮显示。
> 5. 整个代码的目的是让用户选择一个边，并返回该边的标签，以便后续代码可以进行进一步处理。
> 6. 该代码使用NX提供的UF API来实现，具有很好的NX二次开发示范作用。
>
> 总的来说，这段代码实现了在NX中通过二次开发让用户选择一个边并返回标签的功能，体现了NX二次开发的基本方法和思路。
>
