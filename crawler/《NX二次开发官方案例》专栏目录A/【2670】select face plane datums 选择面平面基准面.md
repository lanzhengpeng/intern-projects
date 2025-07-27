### 【2670】select face plane datums 选择面平面基准面

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_face_plane_datums(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[3] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                        { UF_plane_type, 0, 0 },  
                        { UF_datum_plane_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 3, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_face_plane_datums(char *prompt, tag_t **items)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select face/plane/datum", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART,  
            mask_for_face_plane_datums, NULL, &resp, &cnt, items));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*items)[ii], FALSE));  
        return cnt;  
    }

```

#### 代码解析

> 这段NX二次开发代码实现了以下功能：
>
> 1. 选择功能：定义了一个用于选择面/平面/基准平面的选择功能，包括选择范围和选择类型。
> 2. 选择掩码：定义了选择掩码，用于指定选择的面、平面和基准平面的类型。
> 3. 选择对话框：使用UF_UI_select_with_class_dialog函数打开选择对话框，用户可以在对话框中根据掩码选择面/平面/基准平面。
> 4. 取消高亮显示：在选择完成后，遍历选择的项目，取消其高亮显示。
> 5. 返回选择数量：返回用户选择的项目的数量。
> 6. 参数化：使用参数化方式定义选择掩码，方便后续修改和扩展选择类型。
> 7. 函数封装：将选择过程封装为select_face_plane_datums函数，方便在后续代码中调用。
> 8. 错误处理：在函数中添加错误处理，确保函数能够正常执行。
>
> 综上所述，这段代码通过封装和参数化，实现了选择面/平面/基准平面的功能，便于在NX二次开发中使用。
>
