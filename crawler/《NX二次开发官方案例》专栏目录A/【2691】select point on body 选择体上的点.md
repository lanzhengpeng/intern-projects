### 【2691】select point on body 选择体上的点

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_point_on_body(char *prompt, double loc[3], tag_t *face)  
    {  
        int  
            n,  
            resp;  
        double  
            cp[3],  
            identity[16],  
            vmx[9];  
        tag_t  
            body,  
            view;  
        char  
            vname[MAX_ENTITY_NAME_SIZE+1];  
        UF_MODL_ray_hit_point_info_t  
            *hit_list;  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        *face = NULL_TAG;  
        UF_CALL(UF_UI_select_with_single_dialog("Select point on a body", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp,  
            &body, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED)  
        {  
            UF_CALL(UF_DISP_set_highlight(body, 0));  
            UF_CALL(UF_OBJ_ask_name(view, vname));  
            UF_CALL(uc6433(vname, vmx));  
            UF_MTX4_identity(identity);  
            UF_VEC3_copy(cp, loc);  
            UF_VEC3_negate(&vmx[6], vmx);  
            UF_CALL(UF_MODL_trace_a_ray(1, &body, cp, vmx, identity, 1, &n,  
                &hit_list));  
            if (n == 0)  
                UF_CALL(UF_MODL_trace_a_ray(1, &body, cp, &vmx[6], identity, 1, &n,  
                    &hit_list));  
            if (n > 0)  
            {  
                *face = hit_list[0].hit_face;  
            /*  The following command is a work around for PR 4771722 译:这个命令是为了解决PR 4771722的变通方法。 */  
                if (UF_ASSEM_is_occurrence(body))  
                    *face = UF_ASSEM_find_occurrence(  
                        UF_ASSEM_ask_part_occurrence(body), *face);  
                UF_VEC3_copy(hit_list[0].hit_point, loc);  
                UF_CALL(UF_DISP_display_temporary_point(view,  
                    UF_DISP_USE_VIEW_TAG, loc, &attrib, UF_DISP_ASTERISK));  
                UF_free(hit_list);  
            }  
            return body;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码实现了在NX中从对话框选择一个实体，并在该实体上选择一个点的功能。关键步骤包括：
>
> 1. 设置选择掩码，限制只能选择实体特征。
> 2. 弹出对话框，提示用户选择一个点。
> 3. 如果用户选择了点，获取该点在模型空间中的坐标。
> 4. 在该点上绘制一个临时标记。
> 5. 查找该点所在的实体面。
> 6. 如果选择的实体是组件中的实例，则查找面在组件中的实例。
> 7. 输出该点所在的实体和面。
>
> 代码中的主要函数包括：
>
> 总体来说，这段代码实现了NX中在实体上选择点的通用功能，具有很高的实用价值。
>
