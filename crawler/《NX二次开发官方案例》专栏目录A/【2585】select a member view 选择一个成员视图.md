### 【2585】select a member view 选择一个成员视图

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_views(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[1] = { { UF_view_type, UF_all_subtype, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_member_view(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a view", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_views, NULL, &resp,  
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

> 这段代码是用于在NX中二次开发时选择视图的示例代码。
>
> 主要功能包括：
>
> 1. 定义一个函数mask_for_views，用于设置选择过滤条件，只选择视图类型的对象。
> 2. 定义一个函数select_a_member_view，用于弹出一个对话框，让用户选择一个视图对象。在选择前会清除当前的过滤条件，并设置只选择视图的过滤条件。
> 3. 在select_a_member_view中，使用UF_UI_select_with_single_dialog函数弹出选择对话框，用户可以在这个对话框中选择一个视图对象。
> 4. 选择完成后，使用UF_DISP_set_highlight函数取消高亮显示，然后返回所选视图的tag。
> 5. 如果用户没有选择任何对象，则select_a_member_view返回NULL_TAG。
>
> 通过这个示例，展示了在NX二次开发中选择特定类型对象的方法。
>
