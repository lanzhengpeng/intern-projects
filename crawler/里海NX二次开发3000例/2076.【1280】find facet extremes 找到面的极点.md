### 【1280】find facet extremes 找到面的极点

#### 代码

```cpp
    static void map_point_to_csys(tag_t csys, double *pos)  
    {  
        int  
            irc;  
        tag_t  
            matrix;  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys_mx[12] = { 0,0,0,0,0,0,0,0,0,0,0,0},  
            mx[12],  
            origin[3];  
        UF_CALL(UF_CSYS_ask_csys_info(csys, &matrix, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(matrix, &csys_mx[3]));  
        FTN(uf5940)(abs_mx, csys_mx, mx, &irc);  
        FTN(uf5941)(pos, mx);  
    }  
    static void map_point_from_csys(tag_t csys, double *pos)  
    {  
        int  
            irc;  
        tag_t  
            matrix;  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys_mx[12] = { 0,0,0,0,0,0,0,0,0,0,0,0},  
            mx[12],  
            origin[3];  
        UF_CALL(UF_CSYS_ask_csys_info(csys, &matrix, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(matrix, &csys_mx[3]));  
        FTN(uf5940)(csys_mx, abs_mx, mx, &irc);  
        FTN(uf5941)(pos, mx);  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static void find_facet_extremes(tag_t model, tag_t csys, double *llc,  
        double *urc, logical *first)  
    {  
        int  
            ii,  
            facet_id = UF_FACET_NULL_FACET_ID,  
            max_verts,  
            n;  
        double  
            point[3],  
            *verts;  
        if (!(*first))  
        {  
            map_point_to_csys(csys, llc);  
            map_point_to_csys(csys, urc);  
        }  
        UF_CALL(UF_FACET_ask_max_facet_verts(model, &max_verts));  
        UF_CALL(allocate_memory(max_verts * 3 * sizeof(double), (void **)&verts));  
        while (!UF_CALL(UF_FACET_cycle_facets(model, &facet_id)) &&  
            (facet_id != UF_FACET_NULL_FACET_ID))  
        {  
            UF_CALL(UF_FACET_ask_vertices_of_facet(model, facet_id, &n,  
                (double (*)[3]) verts));  
            for (ii = 0; ii < n; ii++)  
            {  
                UF_VEC3_copy(&verts[ii*3], point);  
                map_point_to_csys(csys, point);  
                if (*first)  
                {  
                    UF_VEC3_copy(point, llc);  
                    UF_VEC3_copy(point, urc);  
                    *first = FALSE;  
                }  
                else  
                {  
                    if ( point[0] > urc[0] ) urc[0] = point[0];  
                    if ( point[1] > urc[1] ) urc[1] = point[1];  
                    if ( point[2] > urc[2] ) urc[2] = point[2];  
                    if ( point[0] < llc[0] ) llc[0] = point[0];  
                    if ( point[1] < llc[1] ) llc[1] = point[1];  
                    if ( point[2] < llc[2] ) llc[2] = point[2];  
                }  
            }  
        }  
        UF_free(verts);  
        map_point_from_csys(csys, llc);  
        map_point_from_csys(csys, urc);  
        return;  
    }

```

#### 代码解析

> 这段代码是用于NX CAD系统的二次开发代码，其主要功能是遍历NX模型的所有面，计算模型在指定坐标系中的边界框。代码主要包括以下部分：
>
> 1. 坐标系变换函数：map_point_to_csys和map_point_from_csys用于将点从一个坐标系变换到另一个坐标系。这两个函数通过NX提供的API获取坐标系的信息，并进行变换。
> 2. 内存分配函数：allocate_memory用于分配内存。
> 3. 计算边界框的函数：find_facet_extremes遍历模型的所有面，计算每个面顶点在指定坐标系中的坐标，并更新模型的边界框。边界框初始为第一个面的顶点，然后不断更新。
> 4. 边界框更新：在遍历每个面时，将面的顶点坐标变换到指定坐标系，并与当前边界框进行比较，更新边界框的最大值和最小值。
> 5. 坐标系变换：在计算完边界框后，将边界框的坐标变换回原始坐标系。
> 6. 内存释放：在计算完成后释放分配的内存。
>
> 总的来说，这段代码实现了从NX模型中获取面信息，并进行坐标系变换，计算出模型在指定坐标系中的边界框范围。
>
