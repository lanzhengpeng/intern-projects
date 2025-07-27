### 【1313】fixup view display parms 修正视图显示参数

#### 代码

```cpp
    /*  This function does the same validity checks as the function  
        UF_DRAW_set_view_display in V18.  Some of the data returned by  
        UF_DRAW_ask_view_display does not meet these checks.  
        See PR 4363501.  
    */  
    static void fixup_view_display_parms(UF_DRAW_view_prfs_p_t view_parms)  
    {  
        if (view_parms->hidden_line < UF_DRAW_hidden_line_removal_off ||  
            view_parms->hidden_line > UF_DRAW_hidden_line_removal_on)  
            view_parms->hidden_line = UF_DRAW_hidden_line_removal_off;  
        if (view_parms->edge_hiding_edge < UF_DRAW_edge_hiding_edge_off ||  
            view_parms->edge_hiding_edge > UF_DRAW_edge_hiding_edge_on)  
            view_parms->edge_hiding_edge = UF_DRAW_edge_hiding_edge_off;  
        if (view_parms->smooth < UF_DRAW_smooth_off ||  
            view_parms->smooth > UF_DRAW_smooth_on)  
            view_parms->smooth = UF_DRAW_smooth_off;  
        if (view_parms->silhouettes < UF_DRAW_silhouettes_off ||  
            view_parms->silhouettes > UF_DRAW_silhouettes_on)  
            view_parms->silhouettes = UF_DRAW_silhouettes_off;  
        if (view_parms->uvhatch < UF_DRAW_uvhatch_off ||  
            view_parms->uvhatch > UF_DRAW_uvhatch_on)  
            view_parms->uvhatch = UF_DRAW_uvhatch_off;  
        if (view_parms->tolerance < 0.0)  
            view_parms->tolerance = 0.0;  
        if (view_parms->hidden_line_color < 0 || view_parms->hidden_line_color > 15)  
            view_parms->hidden_line_color = 0;  
        if (view_parms->hidden_line_font < 0 || view_parms->hidden_line_font > 7)  
            view_parms->hidden_line_font = 0;  
        if (view_parms->hidden_line_width < -1 || view_parms->hidden_line_width > 2)  
            view_parms->hidden_line_width = -1;  
        if (view_parms->smooth_edge_color < 0 || view_parms->smooth_edge_color > 15)  
            view_parms->smooth_edge_color = 0;  
        if (view_parms->smooth_edge_font < 0 || view_parms->smooth_edge_font > 7)  
            view_parms->smooth_edge_font = 0;  
        if (view_parms->smooth_edge_width < -1 || view_parms->smooth_edge_width > 2)  
            view_parms->smooth_edge_width = -1;  
        if (view_parms->smooth_edge_gap < UF_DRAW_gap_off ||  
            view_parms->smooth_edge_gap > UF_DRAW_gap_on )  
            view_parms->smooth_edge_gap = UF_DRAW_gap_off;  
        if (view_parms->smooth_edge_gap_size < 0.0 )  
            view_parms->smooth_edge_gap_size = 0.0;  
        if (view_parms->virtual_intersect < UF_DRAW_virtual_intersect_off ||  
            view_parms->virtual_intersect > UF_DRAW_virtual_intersect_on )  
            view_parms->virtual_intersect = UF_DRAW_virtual_intersect_off;  
        if (view_parms->virtual_intersect_color < 0 ||  
            view_parms->virtual_intersect_color > 15)  
            view_parms->virtual_intersect_color = 0;  
        if (view_parms->virtual_intersect_font < 0 ||  
            view_parms->virtual_intersect_font > 7)  
            view_parms->virtual_intersect_font = 0;  
        if (view_parms->virtual_intersect_width < -1 ||  
            view_parms->virtual_intersect_width > 2)  
            view_parms->virtual_intersect_width = -1;  
        if (view_parms->virtual_intersect_gap < UF_DRAW_gap_off ||  
            view_parms->virtual_intersect_gap > UF_DRAW_gap_on )  
            view_parms->virtual_intersect_gap = UF_DRAW_gap_off;  
        if (view_parms->virtual_intersect_gap_size < 0.0 )  
            view_parms->virtual_intersect_gap_size = 0.0;  
        if (view_parms->extracted_edges != UF_DRAW_extracted_edges_on &&  
            view_parms->extracted_edges != UF_DRAW_extracted_edges_off )  
            view_parms->extracted_edges = UF_DRAW_extracted_edges_on;  
        if (view_parms->visible_line_color < 0 ||  
            view_parms->visible_line_color > 15)  
            view_parms->visible_line_color = 0;  
        if (view_parms->visible_line_font < 0 || view_parms->visible_line_font > 7)  
            view_parms->visible_line_font = 0;  
        if (view_parms->visible_line_width < -1 ||  
            view_parms->hidden_line_width > 2)  
            view_parms->visible_line_width = -1;  
    }

```

#### 代码解析

> 这是段用于修正NX视图显示参数的二次开发代码。主要功能包括：
>
> 1. 检查并修正隐藏线显示参数，确保其值在允许的范围内。
> 2. 检查并修正边隐藏参数，确保其值在允许的范围内。
> 3. 检查并修正平滑显示参数，确保其值在允许的范围内。
> 4. 检查并修正轮廓显示参数，确保其值在允许的范围内。
> 5. 检查并修正UV图案显示参数，确保其值在允许的范围内。
> 6. 检查并修正公差参数，确保其值在允许的范围内。
> 7. 检查并修正隐藏线颜色、字体、宽度等参数，确保其值在允许的范围内。
> 8. 检查并修正平滑边颜色、字体、宽度等参数，确保其值在允许的范围内。
> 9. 检查并修正虚拟交线参数，确保其值在允许的范围内。
> 10. 检查并修正提取边参数，确保其值在允许的范围内。
> 11. 检查并修正可见线颜色、字体、宽度等参数，确保其值在允许的范围内。
>
> 通过修正这些参数，可以确保视图显示参数符合NX的规范要求，避免出现错误。
>
