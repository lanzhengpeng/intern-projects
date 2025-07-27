### 【2715】select xhatch 选择图案

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_xhatch(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = {{ UF_drafting_entity_type, UF_draft_crosshatch_subtype, 0 },  
                       { UF_solid_section_type, 0, 0 }};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_xhatch(char *prompt)  
    {  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        double  
            cp[3];  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_xhatch, NULL, &resp,  
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

> 这段代码是用于NX二次开发的一个选择功能，其功能是让用户选择一个曲线实体或剖面线。主要步骤包括：
>
> 1. 定义了一个选择掩码，只允许选择曲线实体和剖面线。
> 2. 调用UF_UI_set_cursor_view来设置光标视图。
> 3. 调用UF_UI_select_with_single_dialog来弹出选择对话框，并让用户选择。
> 4. 如果用户成功选择，则返回选择的实体标签；否则返回NULL_TAG。
> 5. 最后，调用UF_DISP_set_highlight来取消高亮显示。
>
> 总的来说，这段代码实现了NX中的选择曲线或剖面线的功能，为后续的操作奠定了基础。
>
