### 【2713】select things 选择物体

#### 代码

```cpp
    /* 里海 */  
    static int mask_add_faces_and_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_things(char *prompt, tag_t **objects)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_class_dialog( "Select Objects", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_add_faces_and_edges, NULL, &resp,  
            &cnt, objects));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*objects)[ii], 0));  
        return cnt;  
    }

```

#### 代码解析

> 这段NX Open API代码用于实现以下功能：
>
> 1. 定义了一个函数mask_add_faces_and_edges，用于设置选择掩码，允许用户在选择对话框中选择实体类型为“固体”的面和边。
> 2. 定义了一个函数select_things，用于弹出选择对话框，允许用户在工作部件中选择满足掩码条件的对象。
> 3. 在select_things函数中，首先设置了选择光标视图为0，以避免选择对话框打开时NX视图界面卡住。
> 4. 调用了UF_UI_select_with_class_dialog函数，弹出选择对话框，允许用户选择满足掩码条件的对象，并将选择的数量和对象数组返回给变量cnt和objects。
> 5. 选择完成后，将选择光标视图设置为1，恢复NX视图界面。
> 6. 最后，遍历objects数组，将每个选中对象的高亮显示关闭。
>
> 综上所述，这段代码实现了在NX中选择满足特定类型的面和边的功能，并通过选择对话框交互式地获取用户的选择。
>
