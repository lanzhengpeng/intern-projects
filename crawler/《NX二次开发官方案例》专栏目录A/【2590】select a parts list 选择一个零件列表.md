### 【2590】select a parts list 选择一个零件列表

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_parts_list(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_tabular_note_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_parts_list(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_parts_list, NULL, &resp,  
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

> 这段代码是NX二次开发的示例，主要实现了以下功能：
>
> 1. 定义选择零件列表的遮罩函数mask_for_parts_list，用于设置选择对话框的过滤条件，仅允许选择表格注释类型的对象。
> 2. 定义选择零件列表的函数select_a_parts_list，该函数使用单选对话框，提示用户选择一个零件列表对象。在选择后，会清除高亮显示，并返回所选对象的标签。
> 3. select_a_parts_list函数首先调用NX提供的UF_UI_select_with_single_dialog函数，使用单选对话框让用户选择对象。在选择完成后，调用UF_DISP_set_highlight函数清除高亮显示。
> 4. 返回选择结果，如果用户选择了一个对象，则清除高亮显示并返回对象标签；如果没有选择，则返回NULL_TAG。
>
> 总体来说，这段代码实现了在NX中选择特定类型对象（表格注释）的功能，并提供了清除高亮显示的选项。
>
