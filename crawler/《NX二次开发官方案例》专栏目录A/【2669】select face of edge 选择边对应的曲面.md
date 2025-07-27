### 【2669】select face of edge 选择边对应的曲面

#### 代码

```cpp
    static logical is_item_on_list(uf_list_p_t object_list, tag_t item)  
    {  
        int  
            ii;  
        uf_list_p_t  
            temp;  
        for (ii = 0, temp = object_list; temp != NULL; temp = temp->next, ii++)  
            if (temp->eid == item) return TRUE;  
        return FALSE;  
    }  
    /* 里海 */  
    static int filter_list_objects(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        uf_list_p_t  
            the_list = (uf_list_p_t)data;  
        if (is_item_on_list(the_list, object))  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    /* 里海 */  
    static int mask_for_face_in_list(UF_UI_selection_p_t select, void *data)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_list_objects, NULL, data)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_face_in_list(char *prompt, uf_list_p_t list)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select list face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_face_in_list, list, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        return NULL_TAG;  
    }  
    static tag_t select_face_of_edge(tag_t edge)  
    {  
        tag_t  
            face;  
        uf_list_p_t  
            facelist;  
        UF_CALL(UF_MODL_ask_edge_faces(edge, &facelist));  
        face = select_face_in_list("Select face of edge", facelist);  
        UF_CALL(UF_MODL_delete_list(&facelist));  
        return face;  
    }

```

#### 代码解析

> 这段代码是用于在NX中实现选择边所对应面的功能的二次开发代码。具体包括以下几个部分：
>
> 1. is_item_on_list函数：用于判断指定的项目是否在链表中。它通过遍历链表，并比较链表中的元素与给定项目的eid，返回布尔值结果。
> 2. filter_list_objects函数：用于过滤选择对象。它根据传入的链表，判断选择的对象是否在链表中，若在则接受，否则拒绝。
> 3. mask_for_face_in_list函数：用于设置选择掩码，限制只能选择面特征。同时，设置自定义的选择过滤函数filter_list_objects，以限制选择范围。
> 4. select_face_in_list函数：用于在链表中选择一个面。它弹出选择对话框，并设置选择掩码和过滤函数，然后等待用户选择，最后返回选择的面标签。
> 5. select_face_of_edge函数：用于选择边对应的任意一个面。它首先获取边对应的面的链表，然后调用select_face_in_list在链表中选择一个面，并返回选择结果。
>
> 通过以上函数的调用，可以方便地在NX中选择边对应的任意一个面，实现了选择边对应面的功能。
>
