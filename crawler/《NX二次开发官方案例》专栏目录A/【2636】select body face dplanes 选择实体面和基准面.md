### 【2636】select body face dplanes 选择实体面和基准面

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_body_face_dplane(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[3] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                        { UF_datum_plane_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 3, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_body_face_dplanes(char *prompt, tag_t **items)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select face/plane/datum", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART,  
            mask_for_body_face_dplane, NULL, &resp, &cnt, items));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*items)[ii], FALSE));  
        return cnt;  
    }

```

#### 代码解析

> 这段代码是用于在NX中实现选择体的面、平面或基准面的功能。
>
> 代码的主要步骤如下：
>
> 1. 定义了一个选择掩码数组mask，其中包含了三种类型的选择：体、面和基准面。这三种类型的选择在函数mask_for_body_face_dplane中被启用，其他类型的选择则被禁用。
> 2. 定义了选择函数select_body_face_dplanes，用于在当前工作部件中选择面、平面或基准面。函数中调用了NX提供的UF_UI_select_with_class_dialog接口，弹出选择对话框，并传入自定义的选择掩码。
> 3. 在选择结束后，调用UF_DISP_set_highlight函数，关闭高亮显示，因为NX默认会对选中的对象进行高亮显示，关闭后可以避免影响后续操作。
> 4. 选择函数返回选择对象的个数，如果没有选择任何对象，则返回0。
>
> 通过这段代码，可以方便地在NX中实现特定类型的选择，提高了NX二次开发的效率。
>
