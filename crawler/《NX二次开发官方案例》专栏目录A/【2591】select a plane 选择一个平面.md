### 【2591】select a plane 选择一个平面

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_planes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_plane_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_plane(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a plane", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_planes, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX二次开发中用于选择平面的函数。
>
> 首先，mask_for_planes函数用于设置选择集的选择掩码，只允许选择平面类型的对象。
>
> 其次，select_a_plane函数用于弹出对话框，提示用户选择一个平面。在选择之前，会调用mask_for_planes设置选择掩码。用户选择后，会高亮显示所选平面，并返回平面对象的tag。
>
> 整体来看，这段代码实现了在NX中选择平面的功能，通过设置选择掩码和弹出选择对话框来实现。
>
