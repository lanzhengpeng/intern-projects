### 【2668】select face in list 在列表中选择面

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

```

#### 代码解析

> 这段代码是NX Open API的二次开发代码，用于在NX中选择列表中的面。具体来说，代码实现了以下几个功能：
>
> 1. is_item_on_list函数用于检查一个对象是否在列表中。
> 2. filter_list_objects函数用于过滤选择集，只接受列表中的对象。
> 3. mask_for_face_in_list函数用于设置选择过滤条件，只选择面特征。
> 4. select_face_in_list函数用于弹出一个选择对话框，用户在对话框中选择一个面。该函数会根据提供的列表来过滤选择，只允许选择列表中的面。
>
> 整个代码流程是：先定义一个面列表，然后调用select_face_in_list函数选择列表中的面。该函数会设置选择过滤条件，只允许选择列表中的面。用户选择后，会返回选中的面的tag。
>
> 这段代码实现了在NX中选择列表中特定面的功能，可以用于很多需要根据面列表进行选择操作的二次开发场景。
>
