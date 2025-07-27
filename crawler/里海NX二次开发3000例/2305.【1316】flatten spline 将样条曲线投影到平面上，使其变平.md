### 【1316】flatten spline 将样条曲线投影到平面上，使其变平

#### 代码

```cpp
    static void flatten_spline(tag_t spline)  
    {  
        int  
            n_members;  
        tag_t  
            grp,  
            *members,  
            plane;  
        double  
            point[3],  
            x[3],  
            z[3];  
        UF_CURVE_proj_t  
            data;  
        UF_CALL(UF_MODL_ask_curve_props(spline, 0.5, point, x, x, z, x, x));  
        UF_CALL(UF_MODL_create_plane(point, z, &plane));  
        data.proj_type = 1;  
        UF_CALL(UF_CURVE_create_proj_curves(1,&spline, 1,&plane, 1, &data, &grp));  
        UF_CALL(UF_GROUP_ask_group_data(grp, &members, &n_members));  
        UF_CALL(UF_GROUP_ungroup_all(grp));  
        if (n_members > 0)  
        {  
            UF_CALL(UF_OBJ_replace_object_data(spline, members[0]));  
            UF_free(members);  
        }  
        UF_CALL(UF_OBJ_delete_object(plane));  
    }

```

#### 代码解析

> 这是段NX Open C++ API编写的二次开发代码，其功能是平铺样条曲线。具体步骤如下：
>
> 1. 获取样条曲线在中间点的位置、方向等信息，并创建一个垂直于样条曲线的平面。
> 2. 使用投影功能，将样条曲线投影到平面上，得到一组投影曲线。
> 3. 获取投影曲线组中的所有曲线，并删除曲线组。
> 4. 如果投影曲线组中存在曲线，用第一条投影曲线替换原始样条曲线。
> 5. 删除创建的平面。
> 6. 经过以上步骤，原始的样条曲线被平铺成了多条直线段，实现了样条曲线的平铺效果。
>
> 这段代码展示了如何利用NX的曲线投影功能来实现样条曲线的平铺效果，为后续的二次开发提供了参考。
>
