### 【2697】select rcps 选择路由控制点

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_rcps(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_route_control_point_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_rcps(char *prompt, tag_t **rcps)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select Routing Control Points",  
            prompt, UF_UI_SEL_SCOPE_WORK_PART, mask_for_rcps, NULL, &resp,  
            &cnt, rcps));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*rcps)[ii], FALSE));  
        return cnt;  
    }

```

#### 代码解析

> 这段代码是NX二次开发代码，实现了以下功能：
>
> 1. 定义了一个函数mask_for_rcps，用于设置选择过滤条件，仅选择路由控制点。
> 2. 定义了一个函数select_rcps，用于显示选择对话框，并获取用户选择的多个路由控制点。
> 3. select_rcps函数首先使用UF_UI_select_with_class_dialog显示选择对话框，并传入自定义的过滤条件mask_for_rcps。用户在选择完毕后，通过cnt和rcps参数获取用户选择的控制点数量和对象数组。
> 4. 在select_rcps函数中，通过循环调用UF_DISP_set_highlight函数取消每个选择对象的高亮显示。
> 5. 最终，select_rcps函数返回用户选择的控制点数量。
>
> 综上所述，这段代码实现了在NX中显示对话框让用户选择多个路由控制点的功能，并通过自定义过滤条件保证了只选择路由控制点。
>
