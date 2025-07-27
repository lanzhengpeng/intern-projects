### 【0348】ask section lines parent view 询问截面线的父视图

#### 代码

```cpp
    static tag_t ask_section_lines_parent_view(tag_t sxline)  
    {  
        int  
            num_legl_sxsegs = 0,  
            num_sxsegs = 0,  
            num_sxviews = 0;  
        double  
            arrow_dir[3],  
            step_dir[3];  
        tag_t  
            parent_view = NULL_TAG,  
            *sxsegs,  
            *sxviews;  
        UF_DRAW_sxline_type_t  
            sxline_type;  
        UF_DRAW_sxline_status_t  
            sxline_status;  
        UF_DRAW_sxline_leg_t  
            cut_plane_leg;  
        UF_DRF_object_t  
            rotpt_object;  
        UF_CALL(UF_DRAW_ask_sxline_type(sxline, &sxline_type));  
        switch (sxline_type)  
        {  
            case UF_DRAW_simple_sxline:  
                UF_CALL(UF_DRAW_ask_simple_sxline(sxline, step_dir, arrow_dir, &parent_view,  
                    &num_sxviews, &sxviews, &num_sxsegs, &sxsegs, &sxline_status));  
                break;  
            case UF_DRAW_stepped_sxline:  
                UF_CALL(UF_DRAW_ask_stepped_sxline(sxline, step_dir, arrow_dir, &parent_view,  
                    &num_sxviews, &sxviews, &num_sxsegs, &sxsegs, &sxline_status));  
                break;  
            case UF_DRAW_revolved_sxline:  
                UF_CALL(UF_DRAW_ask_revolved_sxline(sxline, step_dir, arrow_dir, &parent_view,  
                    &rotpt_object, &num_sxviews, &sxviews, &num_sxsegs,  
                    &num_legl_sxsegs, &cut_plane_leg, &sxsegs, &sxline_status));  
                break;  
            case UF_DRAW_half_sxline:  
                UF_CALL(UF_DRAW_ask_half_sxline(sxline, step_dir, arrow_dir, &parent_view,  
                    &num_sxviews, &sxviews, &num_sxsegs, &sxsegs, &sxline_status));  
                break;  
            case UF_DRAW_unfolded_sxline:  
                UF_CALL(UF_DRAW_ask_unfolded_sxline(sxline, step_dir, arrow_dir, &parent_view,  
                    &num_sxviews, &sxviews, &num_sxsegs, &sxsegs, &sxline_status));  
                break;  
            case UF_DRAW_breakline:  
                // the view with the break is the section view itself  
                break;  
            case UF_DRAW_folded_sxline:  
                // This is new in NX9.0.3 - comment it out for earlier NX versions!  
                // UF_CALL(UF_DRAW_ask_folded_sxline(sxline, step_dir, arrow_dir,  
                // &parent_view, &num_sxviews, &sxviews, &num_sxsegs, &sxsegs,  
                // &sxline_status));  
                break;  
            default:  
                break;  
        }  
        if (num_sxviews > 0) UF_free(sxviews);  
        if (num_sxsegs > 0) UF_free(sxsegs);  
        return parent_view;  
    }

```

#### 代码解析

> 这段NX二次开发代码用于查询NX中的剖面线视图。
>
> 主要功能包括：
>
> 1. 查询剖面线的类型，包括简单剖面线、阶梯剖面线、旋转剖面线、半剖面线、展开剖面线等。
> 2. 针对不同类型的剖面线，调用不同的函数来查询其参数，如步进方向、箭头方向、父视图、视图列表、段列表等。
> 3. 对于旋转剖面线，还会查询旋转点对象、合法段数量等信息。
> 4. 对于展开剖面线，会查询展开方向等信息。
> 5. 对于折叠剖面线，会查询折叠方向等信息（需要注意，折叠剖面线是NX9.0.3新增的，所以需要注释掉这段代码以兼容早期版本）。
> 6. 最后，释放视图和段列表的内存。
> 7. 返回父视图的tag。
>
> 总的来说，这段代码通过查询剖面线的各种参数，实现了获取剖面线视图的功能。
>
