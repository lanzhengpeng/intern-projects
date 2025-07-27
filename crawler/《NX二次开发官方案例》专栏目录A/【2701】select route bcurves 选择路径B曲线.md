### 【2701】select route bcurves 选择路径B曲线

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
    extern int select_route_bcurves(char *prompt, tag_t **segs)  
    {  
        int  
            cnt,  
            ii,  
            n_part_segs,  
            resp;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            *part_segs;  
        uf_list_p_t  
            choices;  
        UF_CALL(UF_ROUTE_ask_part_segs(part, &n_part_segs, &part_segs));  
        if (n_part_segs == 0 ) return FALSE;  
        choices = make_a_list(n_part_segs, part_segs);  
        UF_free(part_segs);  
        UF_CALL(UF_UI_select_with_class_dialog("Select a Route BCurve", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_route_bcurves,  
                (void *)choices, &resp, &cnt, segs));  
        UF_CALL(UF_MODL_delete_list(&choices));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*segs)[ii], FALSE));  
        return cnt;  
    }

```

#### 代码解析

> 这段代码是一个NX二次开发代码，其主要功能是实现选择路径BCurve的功能。下面是代码的主要步骤和功能：
>
> 1. make_a_list函数：创建一个列表，用于存储零件中的BCurve实体。
> 2. is_a_choice函数：实现选择过滤功能，只允许用户选择列表中的BCurve实体。
> 3. mask_for_route_bcurves函数：设置选择掩码，允许选择线、圆、圆锥和样条线。
> 4. select_route_bcurves函数：实现选择路径BCurve的主要功能。获取当前零件中的BCurve实体，并创建列表。设置选择过滤器和选择掩码。弹出选择对话框，让用户选择BCurve。获取用户选择的BCurve实体，并返回其数量。清除BCurve的选中高亮。
> 5. 获取当前零件中的BCurve实体，并创建列表。
> 6. 设置选择过滤器和选择掩码。
> 7. 弹出选择对话框，让用户选择BCurve。
> 8. 获取用户选择的BCurve实体，并返回其数量。
> 9. 清除BCurve的选中高亮。
>
> 总体来说，这段代码利用NX提供的二次开发接口，实现了交互式选择路径BCurve的功能，为后续的路径规划等操作提供了支持。
>
