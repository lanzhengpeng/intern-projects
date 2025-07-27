### 【2699】select round planar face 选择圆形平面面

#### 代码

```cpp
    static tag_t ask_face_first_outer_edge(tag_t face)  
    {  
        tag_t  
            edge;  
        uf_loop_p_t  
            loop_list,  
            temp;  
        if (!UF_CALL(UF_MODL_ask_face_loops(face, &loop_list)) && loop_list)  
        {  
            for (temp = loop_list; temp != NULL; temp = temp->next)  
                if (temp->type == 1)  
                {  
                    edge = temp->edge_list->eid;  
                    break;  
                }  
            UF_CALL(UF_MODL_delete_loop_list(&loop_list));  
            return edge;  
        }  
        else  
            return NULL_TAG;  
    }  
    /* 里海 */  
    static int filter_round_faces(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        int  
            face_type;  
        tag_t  
            edge = ask_face_first_outer_edge(object);  
        UF_CALL(UF_MODL_ask_edge_type(edge, &face_type));  
        if (face_type == UF_MODL_CIRCULAR_EDGE)  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    /* 里海 */  
    static int mask_for_round_planar_face(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_PLANAR_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_round_faces, NULL, NULL)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_round_planar_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select planar face", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART,  
            mask_for_round_planar_face, NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是用于NX的二次开发，主要功能是让用户选择一个圆形平面面。
>
> 主要步骤包括：
>
> 1. 定义一个函数ask_face_first_outer_edge，用于获取一个面的第一个外边。
> 2. 定义一个函数filter_round_faces，用于判断一个面是否是圆形平面面。它通过获取面的第一个外边，并判断边的类型是否为圆形边来过滤出圆形平面面。
> 3. 定义一个函数mask_for_round_faces，用于设置选择圆形平面面的掩码。
> 4. 定义一个函数select_round_planar_face，用于让用户选择一个圆形平面面。它通过对话框提示用户选择，并应用圆形平面面的过滤条件，最终返回用户选择的面。
>
> 这段代码的主要作用是实现圆形平面面的选择功能，并提供了相关的过滤条件，使得用户可以方便地选择所需的圆形平面面。
>
