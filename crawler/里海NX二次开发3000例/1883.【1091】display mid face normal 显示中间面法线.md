### 【1091】display mid face normal 显示中间面法线

#### 代码

```cpp
    static void display_mid_face_normal(tag_t face)  
    {  
        double  
            parm[2],  
            pnt[3],  
            junk[3],  
            norm_dir[3],  
            uvs[4];  
        UF_CALL(UF_MODL_ask_face_uv_minmax(face, uvs));  
        parm[0] = (uvs[0] + uvs[1]) / 2;  
        parm[1] = (uvs[2] + uvs[3]) / 2;  
        UF_CALL(UF_MODL_ask_face_props(face, parm, pnt, junk, junk, junk,  
            junk, norm_dir, junk));  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, pnt, norm_dir, 0);  
    }

```

#### 代码解析

> 这段代码是NX二次开发代码，用于显示NX模型中面元素的中点法向量。其主要功能包括：
>
> 1. 获取面的参数化范围：通过UF_MODL_ask_face_uv_minmax接口获取面的参数化范围(uvs[0], uvs[2])和最大范围(uvs[1], uvs[3])。
> 2. 计算中点参数值：计算面在U向和V向的中点参数值(uvs[0]+uvs[1]/2, uvs[2]+uvs[3]/2)。
> 3. 获取中点坐标和法向量：使用UF_MODL_ask_face_props接口，根据中点参数值，获取面在该点的坐标(pnt)和法向量(norm_dir)。
> 4. 显示法向量：通过UF_DISP_conehead接口在所有激活的视图中显示一个圆锥头形状，表示法向量，其底面中心位于面的中点，方向为面的法向量。
>
> 这段代码通过参数化方法获取面的中点坐标和法向量，并使用圆锥头表示法向量，以便直观显示面的法向量方向，对于检查模型质量和进行模型分析很有帮助。
>
