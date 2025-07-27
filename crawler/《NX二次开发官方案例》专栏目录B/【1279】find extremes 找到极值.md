### 【1279】find extremes 找到极值

#### 代码

```cpp
    static void map_abs2csys(tag_t input_csys, double vec[3])  
    {  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12],  
            mx[12];  
        int  
            irc;  
        tag_t  
            csys_mx;  
        UF_CALL(UF_CSYS_ask_csys_info(input_csys, &csys_mx, csys));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, &csys[3]));  
        FTN(uf5940)(abs_mx, csys, mx, &irc);  
        UF_CALL(irc);  
        FTN(uf5941)(vec, mx);  
        UF_VEC3_sub(vec, csys, vec);  
    }  
    static void find_extremes(tag_t object, tag_t csys, double *mins, double *maxs,  
        logical *first)  
    {  
        tag_t  
            matrix,  
            subent;  
        double  
            dirs[9],  
            point[3],  
            origin[3],  
            xp[3],  
            yp[3],  
            zp[3],  
            xm[3],  
            ym[3],  
            zm[3];  
        UF_CALL(UF_CSYS_ask_csys_info(csys, &matrix, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(matrix, dirs));  
        UF_VEC3_copy(&dirs[0], xp);  
        UF_VEC3_copy(&dirs[3], yp);  
        UF_VEC3_copy(&dirs[6], zp);  
        UF_VEC3_negate(xp, xm);  
        UF_VEC3_negate(yp, ym);  
        UF_VEC3_negate(zp, zm);  
        UF_CALL( UF_MODL_ask_extreme( object, xp, yp, zp, &subent, point ));  
        map_abs2csys(csys, point);  
        if (*first || ( point[0] > maxs[0] )) maxs[0] = point[0];  
        UF_CALL( UF_MODL_ask_extreme( object, xm, ym, zm, &subent, point ));  
        map_abs2csys(csys, point);  
        if (*first || ( point[0] < mins[0] )) mins[0] = point[0];  
        UF_CALL( UF_MODL_ask_extreme( object, yp, zp, xp, &subent, point ));  
        map_abs2csys(csys, point);  
        if (*first || ( point[1] > maxs[1] )) maxs[1] = point[1];  
        UF_CALL( UF_MODL_ask_extreme( object, ym, zm, xm, &subent, point ));  
        map_abs2csys(csys, point);  
        if (*first || ( point[1] < mins[1] )) mins[1] = point[1];  
        UF_CALL( UF_MODL_ask_extreme( object, zp, xp, yp, &subent, point ));  
        map_abs2csys(csys, point);  
        if (*first || ( point[2] > maxs[2] )) maxs[2] = point[2];  
        UF_CALL( UF_MODL_ask_extreme( object, zm, xm, ym, &subent, point ));  
        map_abs2csys(csys, point);  
        if (*first || ( point[2] < mins[2] )) mins[2] = point[2];  
        *first = FALSE;  
        return;  
    }

```

#### 代码解析

> 这段NX Open C++代码用于查询NX模型中对象在指定坐标系中的最小和最大范围。以下是代码的主要功能和流程：
>
> 1. map_abs2csys函数：将绝对坐标系中的点转换到指定坐标系中。它接收输入坐标系和点的坐标，然后转换坐标系，最后计算并返回转换后的坐标。
> 2. find_extremes函数：查询NX模型中对象在指定坐标系中的最小和最大范围。它首先获取坐标系的矩阵和原点，然后沿坐标轴方向查询对象的最小和最大范围，并调用map_abs2csys函数将范围点转换到坐标系中。最后，更新存储的最小和最大范围值。
> 3. 代码流程：通过在指定坐标系中沿x、y、z轴方向查询对象的最小和最大范围点，并转换坐标系，来获取对象在该坐标系中的最小和最大范围。
> 4. 主要函数：UF_CSYS_ask_csys_info：查询坐标系信息UF_CSYS_ask_matrix_values：查询坐标系矩阵值UF_MODL_ask_extreme：查询对象在指定方向上的极值点UF_VEC3_copy：复制向量UF_VEC3_negate：向量取反UF_VEC3_sub：向量相减
> 5. UF_CSYS_ask_csys_info：查询坐标系信息
> 6. UF_CSYS_ask_matrix_values：查询坐标系矩阵值
> 7. UF_MODL_ask_extreme：查询对象在指定方向上的极值点
> 8. UF_VEC3_copy：复制向量
> 9. UF_VEC3_negate：向量取反
> 10. UF_VEC3_sub：向量相减
>
> 总的来说，这段代码实现了NX模型对象在指定坐标系中的范围查询功能，通过坐标系转换，获取了对象在用户坐标系中的精确范围。
>
