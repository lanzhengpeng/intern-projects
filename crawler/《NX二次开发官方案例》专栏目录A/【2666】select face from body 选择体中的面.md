### 【2666】select face from body 选择体中的面

#### 代码

```cpp
    /* 里海 */  
    static int filter_body_faces(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        tag_t  
            face_body,  
            the_body = *(tag_t *)data;  
        UF_CALL(UF_MODL_ask_face_body(object, &face_body));  
        if (face_body == the_body)  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    /* 里海 */  
    static int mask_for_body_faces(UF_UI_selection_p_t select, void *data)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_body_faces, NULL, data)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_face_from_body(char *prompt, tag_t body)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select body face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_body_faces, &body, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 根据代码内容，这是一段NX二次开发的代码，其主要功能是实现了从NX模型中的某个body中选择一个面的功能。
>
> 关键函数和逻辑如下：
>
> 1. filter_body_faces函数：用于过滤选择的面是否属于指定的body。若属于，则接受选择，否则拒绝。
> 2. mask_for_body_faces函数：用于设置选择时的屏蔽条件，仅选择body上的面。
> 3. select_face_from_body函数：该函数是主函数，用于实现选择功能。它首先设置了选择屏蔽条件，然后调用NX的UI选择接口，弹出选择对话框，并等待用户选择一个面。在用户选择后，关闭高亮，并返回选择的面对象。
> 4. UF_DISP_set_highlight函数：用于关闭选择面的高亮显示。
>
> 通过这些函数的组合，实现了从特定body中选择面的功能。用户可以通过选择对话框选择一个面，如果选择的面属于指定的body，则返回该面对象；否则，选择无效。整体流程清晰，代码结构良好。
>
