### 【0352】ask sheet min uv point 询问曲面最小UV点

#### 代码

```cpp
    static tag_t ask_body_first_face(tag_t body)  
    {  
        tag_t  
            face;  
        uf_list_p_t  
            face_list;  
        UF_CALL(UF_MODL_ask_body_faces(body, &face_list));  
        UF_CALL(UF_MODL_ask_list_item(face_list, 0, &face));  
        UF_CALL(UF_MODL_delete_list(&face_list));  
        return face;  
    }  
    static void ask_sheet_min_uv_point(tag_t body, double min_point[3])  
    {  
        tag_t  
            face;  
        double  
            minmaxs[4],  
            minuv[2];  
        UF_EVALSF_p_t  
            eval;  
        UF_MODL_SRF_VALUE_t  
            surf_eval;  
        UF_EVALSF_pos3_t  
            srf_pos;  
        face = ask_body_first_face(body);  
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

> 这段代码是用于NX的二次开发，主要实现了两个功能：
>
> 1. ask_body_first_face: 获取body的第一个face的tag。首先通过UF_MODL_ask_body_faces获取body的所有face列表，然后使用UF_MODL_ask_list_item获取列表的第一个元素，即第一个face的tag，最后释放列表。
> 2. ask_sheet_min_uv_point: 获取body上第一个face的最小UV点坐标。首先调用ask_body_first_face获取第一个face的tag，然后初始化evaluator，并获取该face的UV最小最大值。接着计算UV最小值点对应的3D空间坐标，并返回这个坐标。
>
> 这些功能主要利用了NX的几何查询API，通过几何体的tag，可以查询到其相关的几何信息，如face列表、UV范围等。这段代码为后续的几何分析提供了基础数据支持。
>
