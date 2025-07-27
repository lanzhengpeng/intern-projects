### 【2594】select a polygon edge 选择一个多边形边

#### 代码

```cpp
    /* 里海 */  
    static int mask_for_polygon_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_caegeom_type, UF_caegeom_edge_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_polygon_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a polygon edge", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_polygon_edges, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是用于在NX中实现选择多边形边界的二次开发代码，主要包含以下几个关键点：
>
> 1. UF_caegeom_type和UF_caegeom_edge_subtype：指定了选择类型为CAE几何，子类型为边。
> 2. UF_UI_set_sel_mask：设置了选择掩码，用于控制在选择对话框中可以选中的对象类型。
> 3. UF_UI_select_with_single_dialog：弹出一个单选对话框，用户可以在对话框中选择一个对象。
> 4. mask_for_polygon_edges：定义了一个函数，用于设置选择掩码，只允许选择多边形的边。
> 5. select_a_polygon_edge：定义了一个函数，用于弹出选择对话框，让用户选择一个多边形的边，并返回选中边的tag。
> 6. UF_DISP_set_highlight：用于关闭选中对象的加亮显示。
> 7. NULL_TAG：表示没有选择任何对象时的返回值。
>
> 总的来说，这段代码通过定义选择掩码，弹出一个单选对话框，让用户选择一个多边形的边，并返回选中边的tag，实现了在NX中便捷地选择多边形边界的功能。
>
