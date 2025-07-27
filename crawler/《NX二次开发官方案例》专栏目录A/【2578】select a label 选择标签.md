### 【2578】select a label 选择标签

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_labels(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_drafting_entity_type, UF_draft_label_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_label(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a label", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_labels, NULL, &resp,  
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

> 这段代码实现了在NX中根据特定条件选择标签的功能，具体包括以下部分：
>
> 1. 静态函数 mask_for_labels()：该函数用于设置选择集的选择掩码，只允许选择标签类型，并禁用其他类型。
> 2. 静态函数 select_a_label()：该函数使用UF_UI_select_with_single_dialog()函数弹出选择对话框，用户可以输入提示信息并选择一个标签。选择结果存储在对象变量object中，如果用户成功选择了标签，则返回该标签的tag，否则返回NULL_TAG。
> 3. 使用 UF_DISP_set_highlight()：函数暂时高亮显示选中的标签，以提示用户选择成功。
> 4. 返回值：如果用户成功选择了标签，则返回该标签的tag，否则返回NULL_TAG。
> 5. 代码作用：通过设置选择掩码和对话框，实现了在NX中根据提示信息选择特定类型对象（这里是标签）的功能。
>
> 总的来说，这段代码实现了在NX中通过对话框选择特定类型对象（这里是标签）的功能，通过设置选择掩码，用户只能选择标签类型。
>
