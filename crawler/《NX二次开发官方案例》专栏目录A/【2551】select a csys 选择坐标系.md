### 【2551】select a csys 选择坐标系

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_csys(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_coordinate_system_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_csys(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a coordinate system",  
            prompt, UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_csys, NULL, &resp,  
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

> 这段代码是用于在NX环境中选择一个坐标系统的二次开发代码。代码的主要功能如下：
>
> 1. mask_for_csys函数：该函数用于设置选择过滤条件，确保只能选择坐标系统。它会清除所有选择条件，并启用特定条件以选择坐标系统。
> 2. select_a_csys函数：该函数用于选择一个坐标系统。它会弹出一个对话框，提示用户选择一个坐标系统。在选择后，会高亮显示所选对象，并返回对象的tag。
> 3. UF_UI_select_with_single_dialog函数：该函数用于弹出选择对话框，并根据设置的选择过滤条件进行选择。它返回选择的响应码、对象tag、点坐标和视图。
> 4. UF_DISP_set_highlight函数：该函数用于取消高亮显示所选对象。
>
> 总的来说，这段代码实现了在NX环境中通过弹出对话框的方式，让用户选择一个坐标系统，并返回所选坐标系统的tag，以便后续进行其他操作。
>
