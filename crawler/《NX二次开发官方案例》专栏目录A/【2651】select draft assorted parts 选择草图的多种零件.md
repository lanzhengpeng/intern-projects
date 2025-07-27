### 【2651】select draft assorted parts 选择草图的多种零件

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_draft_assorted_parts(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_drafting_entity_type, UF_draft_assorted_parts_subtype, 0};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_draft_assorted_parts(char *prompt, tag_t **objects)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select draft assorted parts",  
            prompt, UF_UI_SEL_SCOPE_WORK_PART, mask_for_draft_assorted_parts,  
            NULL, &resp, &cnt, objects));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*objects)[ii], FALSE));  
        return cnt;  
    }

```

#### 代码解析

> 根据代码内容，这是用于在NX中选取草图中特定类型的零件的二次开发代码。具体来说：
>
> 1. 函数功能：这段代码定义了两个函数，mask_for_draft_assorted_parts和select_draft_assorted_parts。前者用于设置选择掩码，只允许选择草图中特定类型的零件；后者则用于弹出一个选择对话框，让用户选择零件。
> 2. 代码实现：mask_for_draft_assorted_parts函数通过设置选择掩码，只允许选择草图中类型为UF_draft_assorted_parts_subtype的零件。select_draft_assorted_parts函数则通过调用NX的UI选择接口，弹出一个对话框，并使用设置好的选择掩码，让用户选择零件。选择结束后，取消高亮显示选中的零件。
> 3. 应用场景：这段代码可用于在NX中选取草图中特定类型的零件，例如在二次开发中需要对草图中的特定零件进行操作时，可以使用这段代码来获取用户选择的零件对象。
> 4. 代码注意事项：在使用这段代码时，需要确保NX的版本与代码兼容，以及正确处理错误返回值。
>
