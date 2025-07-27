### 【2642】select coordinate systems 选择坐标系

#### 代码

```cpp
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
    static int select_coordinate_systems(char *prompt, tag_t **csyss)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select coordinate systems", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_csys, NULL, &resp, &cnt, csyss));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*csyss)[ii], FALSE));  
        return cnt;  
    }

```

#### 代码解析

> 根据代码，这是NX二次开发中用于选择坐标系的函数，主要包含以下内容：
>
> 1. mask_for_csys函数：用于设置选择器的掩码，只允许选择坐标系类型。
> 2. select_coordinate_systems函数：用于打开选择对话框，让用户选择坐标系。参数prompt是提示信息，csyss用于接收选择的坐标系数组。函数返回选择的坐标系数量。
> 3. 在select_coordinate_systems函数中，使用UF_UI_select_with_class_dialog函数打开选择对话框，其中mask_for_csys函数用于设置选择器掩码，只允许选择坐标系类型。然后，用户可以在对话框中选择坐标系，函数返回选择结果到csyss数组中，并返回选择数量。
> 4. 最后，使用循环关闭选中坐标系的高亮显示。
>
> 整体来看，这段代码实现了在NX中让用户选择坐标系的功能，通过二次开发接口调用选择器对话框来实现。
>
