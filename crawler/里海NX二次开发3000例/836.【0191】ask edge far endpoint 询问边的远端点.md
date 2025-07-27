### 【0191】ask edge far endpoint 询问边的远端点

#### 代码

```cpp
    static void ask_edge_far_endpoint(tag_t edge, double pos[3])  
    {  
        int  
            cnt;  
        double  
            parm[3],  
            end[3],  
            start[3];  
        UF_CALL(UF_MODL_ask_edge_verts(edge, start, end, &cnt));  
        UF_CALL(UF_MODL_create_curve_from_edge(edge, &edge));  
        UF_CALL(UF_MODL_ask_curve_parm(edge, start, &parm[0], start));  
        UF_CALL(UF_MODL_ask_curve_parm(edge, pos, &parm[1], pos));  
        UF_CALL(UF_MODL_ask_curve_parm(edge, end, &parm[2], end));  
        UF_CALL(UF_OBJ_delete_object(edge));  
        if (fabs(parm[1] - parm[0]) > fabs(parm[2] - parm[1]))  
             UF_VEC3_copy(start, pos);  
        else UF_VEC3_copy(end, pos);  
    }

```

#### 代码解析

> 这段NX Open C++代码的主要功能是获取边线上的一个点，并返回该点距离端点更近的端点位置。
>
> 主要步骤包括：
>
> 1. 调用UF_MODL_ask_edge_verts函数，获取边线的起点和终点坐标。
> 2. 创建一个临时曲线对象，用于后续操作。
> 3. 使用UF_MODL_ask_curve_parm函数，根据给定的点坐标，计算该点在曲线上的参数值。
> 4. 比较该点与起点和终点的参数距离，选择距离更近的端点坐标作为结果。
> 5. 删除临时创建的曲线对象。
> 6. 最终返回距离给定点更近的端点坐标。
>
> 总体来说，这段代码实现了从给定边线上的一个点，获取距离该点更近的端点坐标的功能。
>
