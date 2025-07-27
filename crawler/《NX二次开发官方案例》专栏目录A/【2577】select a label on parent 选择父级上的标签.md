### 【2577】select a label on parent 选择父级上的标签

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_label_on_parent(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_drafting_entity_type, UF_draft_label_on_parent_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_label_on_parent(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a Label On Parent", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_label_on_parent, NULL, &resp,  
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

> 根据提供的代码，这是段NX（Siemens NX）的二次开发代码，主要功能是选择父对象上的标签。具体来说：
>
> 1. mask_for_label_on_parent函数用于设置选择过滤条件，只允许选择父对象上的标签实体。
> 2. select_a_label_on_parent函数通过提示用户选择，从工作部件中选择父对象上的标签实体，并返回标签的tag。
> 3. 在select_a_label_on_parent中，使用了UF_UI_select_with_single_dialog函数，通过对话框让用户选择，并设置选择过滤条件为mask_for_label_on_parent。
> 4. 选择成功后，关闭高亮显示，并返回标签的tag。
> 5. 如果用户取消选择，则返回NULL_TAG。
> 6. 这段代码的主要作用是在NX中提供一个交互式的选择父对象上标签的功能，用户可以输入提示信息，然后从图形区中选择一个标签，代码会返回该标签的tag，以便后续处理。
>
