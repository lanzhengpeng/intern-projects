### 【2597】select a route bcurve 选择一个路径B曲线

#### 代码

```cpp
    static uf_list_p_t make_a_list(int count, tag_t *item_array)  
    {  
        int  
            ii;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        for (ii = 0; ii < count; ii++)  
            UF_CALL(UF_MODL_put_list_item(list, item_array[ii]));  
        return (list);  
    }  
    /* 里海 */  
    static int is_a_choice(tag_t object, int type[3], void *user_data,  
        UF_UI_selection_p_t select)  
    {  
        uf_list_p_t  
            choices = (uf_list_p_t)user_data;  
        for (; choices != NULL; choices = choices->next)  
        {  
            if (object == choices->eid) return UF_UI_SEL_ACCEPT;  
        }  
        return UF_UI_SEL_REJECT;  
    }  
    /* 里海 */  
    static int mask_for_route_bcurves(UF_UI_selection_p_t select, void *user_data)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 } };  
        UF_CALL(UF_UI_set_sel_procs(select, is_a_choice, NULL, user_data));  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    extern tag_t select_a_route_bcurve(char *prompt)  
    {  
        int  
            n_part_segs,  
            resp;  
        tag_t  
            object,  
            part = UF_PART_ask_display_part(),  
            *part_segs,  
            view;  
        double  
            cp[3];  
        uf_list_p_t  
            choices;  
        UF_CALL(UF_ROUTE_ask_part_segs(part, &n_part_segs, &part_segs));  
        if (n_part_segs == 0 ) return FALSE;  
        choices = make_a_list(n_part_segs, part_segs);  
        UF_free(part_segs);  
        UF_CALL(UF_UI_select_with_single_dialog("Select a Route BCurve", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_route_bcurves,  
                (void *)choices, &resp, &object, cp, &view));  
        UF_CALL(UF_MODL_delete_list(&choices));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是用于在NX中实现选择路线BCurve的功能。
>
> 主要功能包括：
>
> 1. 创建选择列表：使用UF_MODL_create_list和UF_MODL_put_list_item函数，根据零件的路线段创建一个选择列表。
> 2. 设置选择器：通过设置UF_UI_set_sel_procs来过滤出列表中的BCurve。
> 3. 设置选择掩码：使用UF_UI_set_sel_mask来限制选择范围为线、圆、椭圆和样条线。
> 4. 弹出选择对话框：调用UF_UI_select_with_single_dialog函数，根据设置的选择器来弹出选择对话框，用户可以从中选择一个BCurve。
> 5. 处理选择结果：根据选择结果返回选择的BCurve的tag，或者返回NULL_TAG表示用户取消了选择。
> 6. 清理资源：使用UF_MODL_delete_list清理选择列表。
>
> 该代码通过设置选择器、选择掩码，以及使用选择对话框，实现了在NX中选择特定类型BCurve的功能，并返回选择的BCurve的tag。
>
