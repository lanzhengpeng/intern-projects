### 【0206】ask face midpoint 查询面中心点

#### 代码

```cpp
    static void ask_face_midpoint(tag_t face, double mid_point[3])  
    {  
        double  
            norm[3],  
            parm[2],  
            radii[2],  
            u1[3],  
            v1[3],  
            u2[3],  
            v2[3],  
            uvs[4];  
        FTN(uf5404)(&face, uvs);  
        parm[0] = (uvs[0] + uvs[2]) / 2;  
        parm[1] = (uvs[1] + uvs[3]) / 2;  
        UF_CALL(UF_MODL_ask_face_props(face, parm, mid_point, u1, u2, v1, v2,   
            norm, radii));  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，用于获取NX模型中指定面的中点坐标。主要步骤如下：
>
> 1. 定义了ask_face_midpoint静态函数，接收面tag和用于存储中点坐标的数组。
> 2. 定义了一些中间变量，用于存储面的属性。
> 3. 调用UF5404函数，获取指定面的参数坐标(u1, v1)和(u2, v2)。
> 4. 计算参数坐标的平均值，得到面的中点参数坐标。
> 5. 调用UF_MODL_ask_face_props函数，根据中点参数坐标计算并返回面的中点坐标。
> 6. 将面的中点坐标存储在传入的数组中。
> 7. 整个函数通过NX Open C++ API实现，可以用于NX Open二次开发中获取面的中点坐标。
>
