### 【0602】create arc using center start and end locations 创建弧线，使用圆心、起点和终点位置

#### 代码

```cpp
    static tag_t create_arc_using_center_start_and_end_locations(double pos[3][3])  
    {  
        int  
            ii;  
        tag_t  
            arc,  
            points[3];  
        uf_list_p_t  
            arc_list;  
        for (ii = 0; ii < 3; ii++)  
            UF_CALL(UF_CURVE_create_point(&pos[ii][0], &points[ii]));  
        UF_CALL(UF_SO_create_arc_center_2_pnts(points[0],  
            UF_SO_update_within_modeling, points, &arc));  
        UF_CALL(UF_SO_set_visibility_option(arc, UF_SO_visible));  
        UF_CALL(UF_MODL_create_list(&arc_list));  
        UF_CALL(UF_MODL_put_list_item(arc_list, arc));  
        UF_CALL(UF_MODL_delete_object_parms(arc_list));  
        UF_CALL(UF_MODL_delete_list(&arc_list));  
        UF_CALL(UF_OBJ_delete_array_of_objects(3, points, NULL));  
        return arc;  
    }

```

#### 代码解析

> 这段代码是用于在NX中创建一个圆弧的二次开发代码。代码的主要功能是使用圆弧的中心点和两个端点位置创建一个圆弧，并将其添加到NX模型中。具体步骤如下：
>
> 1. 定义一个3x3的double数组pos，用于存储圆弧中心点和两个端点的位置坐标。
> 2. 使用UF_CURVE_create_point函数，根据pos中的坐标，创建3个点对象，分别作为圆弧的起点、终点和中心点。
> 3. 使用UF_SO_create_arc_center_2_pnts函数，根据中心点和两个端点，创建一个圆弧对象arc。
> 4. 调用UF_SO_set_visibility_option函数，设置圆弧的可见性为可见。
> 5. 创建一个UF_MODL_list对象，用于管理后续要删除的对象。
> 6. 将创建的圆弧对象arc添加到UF_MODL_list中。
> 7. 调用UF_MODL_delete_object_parms函数，根据UF_MODL_list中的对象，删除相关的参数。
> 8. 调用UF_MODL_delete_list函数，删除UF_MODL_list对象。
> 9. 调用UF_OBJ_delete_array_of_objects函数，删除之前创建的3个点对象。
> 10. 返回创建的圆弧对象arc。
>
> 总体来说，这段代码实现了通过指定圆弧的中心点和两个端点位置来创建一个圆弧，并将其添加到NX模型中的功能。
>
