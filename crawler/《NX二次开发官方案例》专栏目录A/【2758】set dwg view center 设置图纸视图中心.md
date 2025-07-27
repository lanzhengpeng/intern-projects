### 【2758】set dwg view center 设置图纸视图中心

#### 代码

```cpp
    static tag_t ask_drawing_tag(char *dwg_name)  
    {  
        tag_t  
            dwg = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_OBJ_cycle_by_name_and_type(part, dwg_name, UF_drawing_type,  
            FALSE, &dwg));  
        return dwg;  
    }  
    static void set_dwg_view_center(char *dwg, char *view, double new_center[3])  
    {  
        int  
            dcode,  
            dunits,  
            ii,  
            n = 1;  
        tag_t  
            d_tag,  
            v_tag;  
        double  
            bord[4],  
            delta[3],  
            dsize[2],  
            old_center[3];  
        UF_CALL(uc6485(view, bord));  
        UF_CALL(uc6479(dwg, &dunits, &dcode, dsize));  
        UF_CALL(UF_VIEW_ask_tag_of_view_name(view, &v_tag));  
        UF_VIEW_map_model_to_drawing(v_tag, new_center, new_center);  
        UF_CALL(uc6483(dwg, view, old_center));  
        UF_VEC3_sub(new_center, old_center, delta);  
        UF_VEC3_sub(old_center, delta, new_center);  
        while ((new_center[0] < 0) || (new_center[1] < 0) ||  
               (new_center[0] > dsize[0]) || (new_center[1] > dsize[1]))  
        {  
            UF_VEC3_scale(0.5, delta, delta);  
            UF_VEC3_sub(old_center, delta, new_center);  
            n++;  
        }  
        for (ii = 0; ii < n; ii++)  
        {  
            UF_CALL(uc6484(dwg, view, new_center));  
            UF_CALL(uc6486(view, bord));  
        }  
        d_tag = ask_drawing_tag(dwg);  
        UF_CALL(UF_DRAW_update_one_view(d_tag, v_tag));  
        UF_CALL(UF_DISP_regenerate_display());  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发的，其功能是设置图纸视图的中心位置。
>
> 主要步骤包括：
>
> 1. 定义了一个函数ask_drawing_tag，用于根据图纸名称查询图纸对象标签。
> 2. 定义了一个函数set_dwg_view_center，用于设置图纸视图的中心位置。该函数接受图纸名称、视图名称和新的中心点坐标作为参数。
> 3. 在set_dwg_view_center函数中，首先查询图纸和视图的尺寸、单位和边界等信息。
> 4. 将模型坐标系下的新中心点转换到图纸坐标系下。
> 5. 计算新旧中心点之间的偏移量，并通过循环调整新中心点位置，确保其在图纸视图范围内。
> 6. 更新图纸视图的中心点位置，并刷新显示。
> 7. 最后更新图纸对象，并刷新显示。
>
> 通过这段代码，可以实现图纸视图中心点的动态设置，便于查看图纸的不同区域。
>
