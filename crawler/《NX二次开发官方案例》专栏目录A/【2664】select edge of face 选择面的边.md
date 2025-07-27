### 【2664】select edge of face 选择面的边

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
    static int filter_face_edges(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        tag_t  
            the_face = *(tag_t *)data;  
        uf_list_p_t  
            face_list;  
        UF_CALL(UF_MODL_ask_edge_faces(object, &face_list));  
        if (is_item_on_list(face_list, the_face))  
        {  
            UF_CALL(UF_MODL_delete_list(&face_list));  
            return (UF_UI_SEL_ACCEPT);  
        }  
        else  
        {  
            UF_CALL(UF_MODL_delete_list(&face_list));  
            return (UF_UI_SEL_REJECT);  
        }  
    }  
    /* 里海 */  
    static int mask_for_face_edges(UF_UI_selection_p_t select, void *data)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_face_edges, NULL, data)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_edge_of_face(char *prompt, tag_t face)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select edge of face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_face_edges, &face, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这是段NX二次开发代码，其主要功能是选择一个面上的边。
>
> 代码的核心逻辑包括以下部分：
>
> 1. is_item_on_list函数：用于检查一个边是否属于指定的面。它遍历面的边列表，如果找到匹配的边，则返回TRUE。
> 2. filter_face_edges函数：用于过滤选择集，只选择指定面中的边。它先获取边的面列表，然后检查是否包含指定面，若包含则接受该边，否则拒绝。
> 3. mask_for_face_edges函数：用于设置选择集的掩码，使其只选择实体边。
> 4. select_edge_of_face函数：用于弹出一个对话框，提示用户选择一个面上的边。它设置选择集的过滤函数和掩码，然后获取用户的选择，并返回所选边的tag。
>
> 整体来说，这段代码实现了从指定面中选择边的功能。它先获取面的边列表，然后设置选择集过滤条件，让用户选择，最后返回所选边的tag。
>
