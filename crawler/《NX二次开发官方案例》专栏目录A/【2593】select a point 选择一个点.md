### 【2593】select a point 选择一个点

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_points(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_point_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_point(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a point", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_points, NULL, &resp,  
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

> 这段代码是用于在NX中实现一个功能，让用户选择一个点，然后返回该点的tag。具体来说：
>
> 1. mask_for_points函数：这个函数用于设置选择过滤条件，只允许选择点。通过UF_UI_set_sel_mask函数，将选择过滤条件设置为只允许选择点，并将其应用到当前的选择集。
> 2. select_a_point函数：这个函数负责让用户选择一个点。它首先显示一个选择对话框，让用户在对话框中选择一个点。如果用户成功选择了一个点，就取消该点的高亮，并返回该点的tag。
> 3. 使用方式：调用select_a_point函数，传入提示字符串，即可让用户选择一个点，并返回该点的tag。如果用户没有选择点，则返回NULL_TAG。
> 4. 代码目的：这段代码的目的是提供一个通用的函数，让其他二次开发代码可以轻松让用户选择一个点，并获取该点的tag，从而实现基于点的操作。
> 5. NX二次开发：这段代码展示了NX二次开发中常用的技术，包括使用选择过滤条件、获取用户选择、取消高亮等，这些技术对于实现NX二次开发功能非常有用。
> 6. 提示字符串：select_a_point函数接收一个提示字符串作为参数，这样可以根据需要定制提示用户选择的内容。
>
> 这段代码实现了NX二次开发中一个常见的需求，即让用户选择一个点，并获取该点的tag。其设计简洁且通用，对于NX二次开发很有参考价值。
>
