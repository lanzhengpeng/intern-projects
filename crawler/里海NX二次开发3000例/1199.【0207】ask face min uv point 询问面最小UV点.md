### 【0207】ask face min uv point 询问面最小UV点

#### 代码

```cpp
    static void ask_face_min_uv_point(tag_t face, double min_point[3])  
    {  
        double  
            minmaxs[4],  
            minuv[2];  
        UF_EVALSF_p_t  
            eval;  
        UF_MODL_SRF_VALUE_t  
            surf_eval;  
        UF_EVALSF_pos3_t  
            srf_pos;  
        UF_CALL(UF_EVALSF_initialize(face, &eval));  
        UF_CALL(UF_EVALSF_ask_face_uv_minmax(eval, minmaxs));  
        minuv[0] = minmaxs[0];  
        minuv[1] = minmaxs[2];  
        UF_CALL(UF_EVALSF_evaluate(eval, UF_MODL_EVAL, minuv, &surf_eval));  
        UF_CALL(UF_EVALSF_ask_minimum_face_dist(eval, surf_eval.srf_pos,  
            &srf_pos));  
        UF_VEC3_copy(srf_pos.pnt3, min_point);  
        UF_CALL(UF_EVALSF_free(&eval));  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API的示例，用于计算NX中一个面(face)上的最小UV坐标点，并获取该点在三维空间中的位置。
>
> 主要步骤包括：
>
> 1. 初始化面(face)的评估器(eval)。
> 2. 获取面(face)在UV参数空间中的最小和最大值。
> 3. 取UV坐标的最小值(minuv)。
> 4. 在该UV坐标处评估面(face)。
> 5. 计算面(face)在UV坐标处与原点之间的最小距离，得到一个点(srf_pos)。
> 6. 复制该点(srf_pos)的坐标到输出参数min_point中。
> 7. 释放评估器(eval)。
>
> 通过以上步骤，该函数可以获取NX中一个面(face)上的最小UV坐标点在三维空间中的位置。
>
