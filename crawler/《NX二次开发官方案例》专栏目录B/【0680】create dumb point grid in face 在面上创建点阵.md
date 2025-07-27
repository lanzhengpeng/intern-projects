### 【0680】create dumb point grid in face 在面上创建点阵

#### 代码

```cpp
    /*  
        In this example all of the points will lie in the interior of the face.  
        Use create_dumb_point_grid_on_face if you want the first and last points  
        to fall on the edges of the face.  
    */  
    static void create_dumb_point_grid_in_face(tag_t face, int nu, int nv)  
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
        du = (minmax[1] - minmax[0]) / (nu + 1);  
        dv = (minmax[3] - minmax[2]) / (nv + 1);  
        for (ii = 0, uv[0] = minmax[0] + du; ii < nu; ii++, uv[0] = uv[0] + du)  
        {  
            for (jj = 0, uv[1] = minmax[2] + dv; jj < nv; jj++, uv[1] = uv[1] + dv)  
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

> 这段代码的功能是在NX的面上创建一个二维的点网格。代码的主要逻辑如下：
>
> 1. 获取面的类型，以确定后续创建点的方式。
> 2. 获取面的UV参数范围，并据此计算每个点的UV参数间隔。
> 3. 在给定的UV范围内，按指定间隔遍历每个UV参数，并创建相应的点。
> 4. 对于普通面，直接使用面的评估器来获取点坐标并创建点。
> 5. 对于混合面，先评估面的UV参数，然后使用评估结果创建点。
> 6. 在每次循环中，UV参数都会按指定间隔递增，以确保创建的点均匀分布在面上。
> 7. 使用撤销标记来记录当前状态，以便后续可以撤销操作。
>
> 通过这段代码，用户可以在NX的面上快速创建一个均匀分布的二维点网格，这对于进行后续的网格分析或操作非常有用。
>
