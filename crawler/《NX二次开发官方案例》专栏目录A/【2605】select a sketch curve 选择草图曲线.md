### 【2605】select a sketch curve 选择草图曲线

#### 代码

```cpp
    static tag_t ask_next_sketch(tag_t part, tag_t sketch)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_sketch_type, &sketch));  
        return (sketch);  
    }  
    static void ask_all_sketch_curves(uf_list_p_t curve_list)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *geoms,  
            part = UF_PART_ask_display_part(),  
            sketch = NULL_TAG;  
        while ((sketch = ask_next_sketch(part, sketch)) != NULL_TAG)  
        {  
            UF_CALL(UF_SKET_ask_geoms_of_sketch(sketch, &n, &geoms));  
            for (ii = 0; ii < n; ii++)  
                UF_CALL(UF_MODL_put_list_item(curve_list, geoms[ii]));  
            if (n > 0) UF_free(geoms);  
        }  
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
    static int mask_for_sketch_curves(UF_UI_selection_p_t select, void *choices)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 } };  
        ask_all_sketch_curves((uf_list_p_t)choices);  
        UF_CALL(UF_UI_set_sel_procs(select, is_a_choice, NULL, choices));  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    extern tag_t select_a_sketch_curve(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        uf_list_p_t  
            sketch_curves;  
        UF_CALL(UF_MODL_create_list(&sketch_curves));  
        UF_CALL(UF_UI_select_with_single_dialog("Select a sketch curve", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_sketch_curves,  
                (void *)sketch_curves, &resp, &object, cp, &view));  
        UF_CALL(UF_MODL_delete_list(&sketch_curves));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX Open API编写的二次开发代码，主要实现了以下功能：
>
> 1. ask_next_sketch函数：循环遍历指定部件中的所有草图，并返回下一个草图。
> 2. ask_all_sketch_curves函数：遍历指定部件中的所有草图，并获取每个草图中的所有几何体（线、圆、曲线、样条等），并将它们添加到列表中。
> 3. is_a_choice函数：用于判断选择的对象是否在给定的列表中，如果是则接受选择，否则拒绝。
> 4. mask_for_sketch_curves函数：设置选择对话框的过滤条件，只允许选择草图中的线、圆、曲线和样条。
> 5. select_a_sketch_curve函数：创建一个选择对话框，只允许选择草图中的几何体，用户在对话框中选择一个草图曲线后，函数返回这个曲线的tag。
> 6. 主函数select_a_sketch_curve：创建选择列表，设置过滤条件，打开选择对话框，用户选择后返回选择的草图曲线的tag。
>
> 总体来说，这段代码实现了在NX中选择草图曲线的功能，用户通过对话框选择，代码返回选中的草图曲线的tag。
>
