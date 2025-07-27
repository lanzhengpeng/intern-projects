### 【2647】select datum planes 选择基准平面

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_datum_planes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_datum_plane_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_datum_planes(char *prompt, tag_t **dplanes)  
    {  
        int  
            ii,  
            n,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog( "Select datum planes", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_datum_planes, NULL, &resp,  
            &n, dplanes));  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight((*dplanes)[ii], FALSE));  
        return n;  
    }

```

#### 代码解析

> 这段代码是用于在NX中二次开发，以选择基准平面的功能。
>
> 代码中定义了一个静态函数mask_for_datum_planes，用于设置选择掩码，以便在对话框中只显示基准平面。
>
> 另一个函数select_datum_planes用于创建选择对话框，提示用户选择基准平面，并获取用户选择的基准平面数量和tag。
>
> 在选择完成后，使用UF_DISP_set_highlight函数关闭高亮显示。
>
> 总的来说，这段代码实现了在NX中选择基准平面的功能，通过自定义选择掩码过滤非基准平面，并通过对话框让用户选择所需的基准平面，最后关闭高亮显示。
>
