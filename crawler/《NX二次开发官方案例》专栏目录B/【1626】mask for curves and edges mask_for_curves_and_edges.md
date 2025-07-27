### 【1626】mask for curves and edges mask_for_curves_and_edges

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_curves_and_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[5] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 5, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }

```

#### 代码解析

> 这段NX二次开发代码的作用是设置NX中的选择掩码，用于在图形界面中选择特定类型的曲线和边线。
>
> 代码的关键点包括：
>
> 1. 定义了一个UF_UI_mask_t类型的数组mask，用于存储要选择的曲线和边线类型。
> 2. 数组中包含5个元素，分别对应线、圆、圆锥曲线、样条曲线和实体边线类型。
> 3. 调用UF_UI_set_sel_mask函数，将mask数组中的选择类型应用到参数select表示的选择集中。
> 4. UF_UI_set_sel_mask函数的参数中，第一个参数select是选择集的句柄；第二个参数表示清除当前选择集并设置新的选择掩码；第三个参数是掩码数组的长度；第四个参数是要设置的掩码数组。
> 5. 如果函数调用成功，返回UF_UI_SEL_SUCCESS；如果失败，返回UF_UI_SEL_FAILURE。
> 6. 这个函数的作用是在NX中选择特定类型的曲线和边线，比如在需要选择实体模型中的边线时非常有用。
>
> 总的来说，这段代码的作用是设置选择掩码，以便在NX中选择特定类型的曲线和边线。
>
