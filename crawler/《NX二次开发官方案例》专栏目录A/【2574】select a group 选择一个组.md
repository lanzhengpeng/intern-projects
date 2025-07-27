### 【2574】select a group 选择一个组

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_groups(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_group_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_group(char *prompt)  
    {  
        int  
            ii,  
            member_count,  
            resp;  
        double  
            cp[3];  
        tag_t  
            *member_list,  
            group,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a group", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_groups, NULL, &resp,  
            &group, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(group, 0));  
            UF_CALL(UF_GROUP_ask_group_data(group, &member_list, &member_count));  
            for (ii = 0; ii < member_count; ii++)  
               UF_CALL(UF_DISP_set_highlight(member_list[ii], 0));  
            if (member_count > 0) UF_free(member_list);  
            return group;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，用于在NX装配环境中选择一个组，并高亮显示组内所有成员。
>
> 主要功能包括：
>
> 1. 定义了一个mask_for_groups函数，用于设置选择过滤条件，仅允许选择组。
> 2. 定义了一个select_a_group函数，用于选择一个组，并获取组内的所有成员。
> 3. 使用UF_UI_select_with_single_dialog函数弹出一个单选对话框，让用户选择一个组。
> 4. 获取用户选择的组，并获取组内的所有成员。
> 5. 使用UF_DISP_set_highlight函数高亮显示组内所有成员。
> 6. 如果用户选择了组，则返回组标签，否则返回NULL_TAG。
> 7. 使用UF_GROUP_ask_group_data函数获取组数据。
> 8. 使用UF_DISP_set_highlight函数取消高亮显示。
> 9. 释放内存。
>
> 总的来说，这段代码实现了在NX装配环境中选择一个组，并高亮显示组内所有成员的功能。它通过使用NX Open C++ API中的UF_UI、UF_DISP和UF_GROUP等函数来实现。
>
