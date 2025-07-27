### 【0189】ask edge close endpoint 询问边靠近端点

#### 代码

```cpp
    static void ask_edge_close_endpoint(tag_t edge, double pos[3])  
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
        if (fabs(parm[1] - parm[0]) < fabs(parm[2] - parm[1]))  
             UF_VEC3_copy(start, pos);  
        else UF_VEC3_copy(end, pos);  
    }

```

#### 代码解析

> 这段代码是用于在NX Open中获取边上某点对应端点的二次开发代码。其主要功能如下：
>
> 1. 获取边上的端点：通过UF_MODL_ask_edge_verts函数，可以获取边的两个端点坐标start和end。
> 2. 创建曲线：调用UF_MODL_create_curve_from_edge函数，根据边创建一条曲线，并返回曲线的tag。
> 3. 查询参数：利用UF_MODL_ask_curve_parm函数，可以查询点pos在曲线上对应的参数值parm[1]。同时，也可以查询端点start和end在曲线上的参数值parm[0]和parm[2]。
> 4. 删除曲线：通过UF_OBJ_delete_object函数，删除前面创建的曲线对象。
> 5. 获取对应端点：根据点pos在曲线上的参数值与端点在曲线上的参数值的比较，可以判断pos距离start近还是距离end近。如果距离start近，就将start坐标赋值给pos，否则将end坐标赋值给pos。
>
> 总的来说，这段代码通过查询边上某点的参数值，实现了获取该点对应端点的功能。
>
