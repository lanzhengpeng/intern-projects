### 【2862】show constraint data 显示约束数据

#### 代码

```cpp
    static void show_constraint_data(tag_t tool, tag_t part, char *how_far,  
        double fixed1[2], double fixed2[2], double feature1[2], double feature2[2],  
        double plane_org[3], double plane_xyz[9])  
    {  
        double  
            point[3];  
        report_tag_type_and_subtype("tool", tool);  
        report_tag_type_and_subtype("part", part);  
        ECHOS(how_far);  
        UF_free(how_far);  
        UF_VEC2_copy(fixed1, point);  
        point[2] = 0;  
        map_plane2abs(plane_org, plane_xyz, point);  
        display_temporary_point(point);  
        UF_VEC2_copy(fixed2, point);  
        point[2] = 0;  
        map_plane2abs(plane_org, plane_xyz, point);  
        display_temporary_point(point);  
        UF_VEC2_copy(feature1, point);  
        point[2] = 0;  
        map_plane2abs(plane_org, plane_xyz, point);  
        display_temporary_point(point);  
        UF_VEC2_copy(feature2, point);  
        point[2] = 0;  
        map_plane2abs(plane_org, plane_xyz, point);  
        display_temporary_point(point);  
    }

```

#### 代码解析

> 这段代码是一个NX Open API函数，用于显示约束数据。
>
> 主要功能包括：
>
> 1. 输出工具和零件的标签类型和子类型。
> 2. 输出约束距离描述字符串。
> 3. 将固定的约束点1和2、特征点1和2从平面坐标系转换到绝对坐标系，并显示为临时点。
> 4. 使用UF_VEC2_copy函数从二维数组拷贝点到三维数组，并设置z坐标为0。
> 5. 使用map_plane2abs函数将点从平面坐标系转换到绝对坐标系。
> 6. 使用display_temporary_point函数显示临时点。
>
> 该函数主要用于在NX中显示约束数据，以便于调试和验证约束的正确性。
>
