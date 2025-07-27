### 【2712】select splines 选择样条线

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_splines(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_spline_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_BCURVE_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_splines(char *prompt, tag_t **splines)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select splines", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_splines, NULL, &resp, &cnt,  
            splines));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*splines)[ii], FALSE));  
        return cnt;  
    }

```

#### 代码解析

> 这段代码是用于在NX中选取样条曲线并进行后续处理的一段二次开发代码。具体来说，代码的主要功能包括：
>
> 1. 定义一个用于设置选取样条曲线的遮罩函数mask_for_splines。该函数会清除原有的选取遮罩，并启用特定的选取类型，即样条曲线和实体边界的选取。
> 2. 定义一个用于选取样条曲线的函数select_splines。该函数会弹出一个对话框，提示用户选取样条曲线，并设置选取范围和遮罩。选取结束后，会关闭高亮显示。
> 3. select_splines函数会返回选取的样条曲线数量以及样条曲线的句柄数组。
>
> 通过调用select_splines函数，可以获取用户选取的样条曲线，并进行后续的几何处理或分析。这段代码为二次开发提供了选取样条曲线的便利。
>
