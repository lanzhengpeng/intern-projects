### 【1587】map arc center2wcs 映射弧中心到世界坐标系

#### 代码

```cpp
    static void map_arc_center2wcs(tag_t arc, UF_CURVE_arc_t *arc_data)  
    {  
        tag_t  
            owner,  
            part = UF_PART_ask_display_part(),  
            work = UF_ASSEM_ask_work_part();  
        map_arc_center2abs(arc_data);  
        UF_CALL(UF_OBJ_ask_owning_part(arc, &owner));  
        if (owner == part)  
            UF_CALL(UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, arc_data->arc_center,  
                UF_CSYS_ROOT_WCS_COORDS, arc_data->arc_center));  
        else if (owner == work)  
            UF_CALL(UF_CSYS_map_point(UF_CSYS_WORK_COORDS, arc_data->arc_center,  
                UF_CSYS_ROOT_WCS_COORDS, arc_data->arc_center));  
        else  
            printf("Cannot map\n");  
    }

```

#### 代码解析

> 这段代码实现了从弧线的中心点到工作坐标系（WCS）的映射。
>
> 首先，通过UF_CURVE_ask_arc_data函数获取弧线的中心点坐标。
>
> 接着，通过UF_OBJ_ask_owning_part函数获取弧线的宿主组件。
>
> 然后，根据宿主组件的不同（零件或装配体），使用UF_CSYS_map_point函数将中心点从弧线所在的坐标系映射到WCS。对于零件，从模型坐标系映射到WCS；对于装配体，从工作坐标系映射到WCS。
>
> 最后，如果宿主组件既不是零件也不是装配体，则打印提示信息。
>
> 综上所述，该代码实现了弧线中心点到WCS的映射，以便进行后续的坐标转换等操作。
>
