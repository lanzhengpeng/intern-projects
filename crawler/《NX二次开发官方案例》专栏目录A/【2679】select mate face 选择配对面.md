### 【2679】select mate face 选择配对面

#### 代码

```cpp
    /* 里海 */  
    static int filter_mate_faces(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        int  
            ii = 0,  
            *to_from = (int *)data;  
        tag_t  
            part_occ,  
            work_occ;  
    /*  The occurrence must be under the current work part in the assembly tree 译:发生必须位于装配树中的当前工作部件下。 */  
        work_occ = UF_ASSEM_ask_work_occurrence();  
        part_occ = UF_ASSEM_ask_part_occurrence(object);  
        while ((part_occ != NULL_TAG) && (part_occ != work_occ))  
        {  
            UF_CALL(UF_ASSEM_ask_parent_component(part_occ, &part_occ));  
            ii++;  
        }  
        if ((work_occ != NULL_TAG) && (part_occ == NULL_TAG))  
            return (UF_UI_SEL_REJECT);  
        if ((*to_from == 1) && (ii == 0))  
            return (UF_UI_SEL_REJECT);  
        return (UF_UI_SEL_ACCEPT);  
    }  
    /* 里海 */  
    static int mask_for_mate_face(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_PLANAR_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_mate_faces, NULL, type)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_mate_face(char *prompt, int to_from)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select planar face to mate",  
            prompt, UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_mate_face, &to_from,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是一个用于在NX装配环境中选择平面面的函数。主要包括以下几个部分：
>
> 1. filter_mate_faces函数：用于过滤选择集，只允许选择当前工作部件下的面。
> 2. mask_for_mate_face函数：设置选择器的选择类型为实体和平面面。
> 3. select_mate_face函数：用于打开选择对话框，并返回用户选择的面。
>
> 具体来说，filter_mate_faces函数通过查询装配树，只允许选择当前工作部件下的平面面。mask_for_mate_face函数设置选择器只允许选择实体和平面面。最后，select_mate_face函数打开选择对话框，用户可以在装配环境中选择平面面，并返回用户选择的面的标识。
>
> 这段代码通过自定义选择器，实现了在装配环境中方便地选择平面面，用于后续的配对等操作。
>
