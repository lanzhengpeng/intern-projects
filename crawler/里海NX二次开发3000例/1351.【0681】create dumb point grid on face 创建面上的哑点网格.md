### 【0681】create dumb point grid on face 创建面上的哑点网格

#### 代码

```cpp
    static void create_dumb_point_grid_on_face(tag_t face, int nu, int nv)  
    {  
        int  
            ii, jj, type;  
        tag_t  
            point;  
        double  
            du,  
            dv,  
            minmax[4],  
            uv[2];  
        UF_EVALSF_p_t  
            evaluator;  
        UF_MODL_SRF_VALUE_t  
            surf_eval;  
        set_undo_mark("Grid of points");  
        UF_CALL(UF_MODL_ask_face_type(face, &type));  
        UF_CALL(UF_EVALSF_initialize(face, &evaluator));  
        UF_CALL(UF_EVALSF_ask_face_uv_minmax(evaluator, minmax));  
        du = (minmax[1] - minmax[0]) / (nu - 1);  
        dv = (minmax[3] - minmax[2]) / (nv - 1);  
        for (ii = 0, uv[0] = minmax[0]; ii < nu; ii++, uv[0] = uv[0] + du)  
        {  
            for (jj = 0, uv[1] = minmax[2]; jj < nv; jj++, uv[1] = uv[1] + dv)  
            {  
                if (type == UF_MODL_BLENDING_FACE)  
                    UF_CALL(UF_MODL_evaluate_face(face, UF_MODL_EVAL, uv,  
                        &surf_eval));  
                else  
                    UF_CALL(UF_EVALSF_evaluate(evaluator, UF_MODL_EVAL, uv,  
                        &surf_eval));  
                UF_CALL(UF_CURVE_create_point(surf_eval.srf_pos, &point));  
            }  
        }  
    }

```

#### 代码解析

> 这段代码实现了在NX模型的一个面上创建一个网格点阵的功能。具体步骤如下：
>
> 1. 首先调用UF_MODL_ask_face_type来获取面的类型。
> 2. 然后调用UF_EVALSF_initialize来初始化面的参数化信息。
> 3. 调用UF_EVALSF_ask_face_uv_minmax来获取U和V参数的范围。
> 4. 根据U和V参数范围，以及用户输入的网格尺寸，计算出U和V方向上的步长。
> 5. 在两层循环中，遍历所有参数组合，调用UF_EVALSF_evaluate来计算参数对应的曲面点。
> 6. 最后调用UF_CURVE_create_point来在曲面上创建点。
> 7. 如果是混合面，则使用UF_MODL_evaluate_face来计算参数对应的曲面点。
> 8. 在循环结束后，撤销标记，结束操作。
>
> 这段代码的核心思想是根据参数化信息在面上创建均匀分布的点阵，实现了NX二次开发中的基础功能之一。
>
