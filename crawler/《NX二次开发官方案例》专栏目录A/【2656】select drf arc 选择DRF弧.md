### 【2656】select drf arc 选择DRF弧

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_drf_arcs(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[3] = {{ UF_circle_type, 0, 0 },  
                       {UF_solid_type, 0, UF_UI_SEL_FEATURE_CIRCULAR_EDGE },  
                       { UF_section_edge_type, 0, 0 }};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 3, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static logical select_drf_arc(char *prompt, UF_DRF_object_p_t arc)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog("Select an arc", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_drf_arcs, NULL, &resp,  
            &arc->object_tag, cp, &arc->object_view_tag));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(arc->object_tag, 0));  
            return TRUE;  
        }  
        else return FALSE;  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发中，实现选择DRF弧线功能的函数。
>
> 主要功能包括：
>
> 1. 定义一个数组mask，包含三种弧线类型和对应的选择掩码，用于限制在选择时只能选择这三种类型的弧线。
> 2. 定义mask_for_drf_arcs函数，用于设置选择掩码。
> 3. 定义select_drf_arc函数，用于实现选择DRF弧线的功能。首先清空当前视图的选择集，然后调用NX UI的UF_UI_select_with_single_dialog函数，弹出一个选择对话框，用户可以在对话框中选择一个弧线。选择后，弧线的对象标识和视图标识会被存储在arc结构体中。如果用户成功选择了一个弧线，函数会返回TRUE，否则返回FALSE。
> 4. 在select_drf_arc函数中，还通过UF_DISP_set_highlight函数将选择的弧线高亮显示。
>
> 通过调用select_drf_arc函数，可以方便地在NX中实现选择DRF弧线的功能，返回选中的弧线信息。
>
