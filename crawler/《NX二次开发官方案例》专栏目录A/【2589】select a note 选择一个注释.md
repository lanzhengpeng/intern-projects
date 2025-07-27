### 【2589】select a note 选择一个注释

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_notes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_drafting_entity_type, UF_draft_note_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_note(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog("Select a note", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_notes, NULL, &resp,  
            &object, cp, &view));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是用于在NX中选取注释对象的二次开发代码。具体包括以下功能：
>
> 1. 定义了一个名为mask_for_notes的函数，用于设置选取注释对象的类型掩码。它设置了一个类型为注释的掩码，以指定仅选取注释对象。
> 2. 定义了一个名为select_a_note的函数，用于提示用户选取一个注释对象。它会提示用户输入注释对象，并调用NX提供的选取函数UF_UI_select_with_single_dialog，以获取用户选取的对象。
> 3. 在select_a_note函数中，设置了光标视图，以在选取过程中显示不同的光标。选取完成后，将光标视图恢复。
> 4. 根据用户选取的对象，判断是否成功选取注释对象，并调用UF_DISP_set_highlight函数，以高亮显示选取的注释对象。
> 5. 如果成功选取了注释对象，select_a_note函数会返回该注释对象的tag，否则返回NULL_TAG。
> 6. 在调用select_a_note函数时，需要传递提示信息作为参数。
> 7. 该代码实现了在NX中选取注释对象的功能，可作为二次开发的基础，以便进行后续的注释对象操作。
>
