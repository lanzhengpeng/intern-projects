### 【2820】set wcs by face miduv 根据面中点设置WCS

#### 代码

```cpp
    static tag_t set_wcs_by_face_miduv(tag_t face)  
    {  
        tag_t  
            wcs,  
            mx,  
            orig_wcs;  
        double  
            csys[9],  
            junk[3],  
            origin[3],  
            parm[2],  
            uvs[4],  
            x_dir[3],  
            y_dir[3];  
        UF_CALL(UF_CSYS_ask_wcs(&orig_wcs));  
        UF_CALL(UF_MODL_ask_face_uv_minmax(face, uvs));  
        parm[0] = (uvs[0] + uvs[1]) / 2;  
        parm[1] = (uvs[2] + uvs[3]) / 2;  
        UF_CALL(UF_MODL_ask_face_props(face, parm, origin, x_dir, y_dir,  
            junk, junk, junk, junk));  
        UF_CALL(UF_MTX3_initialize(x_dir, y_dir, csys));  
        UF_CALL(UF_CSYS_create_matrix(csys, &mx));  
        UF_CALL(UF_CSYS_create_temp_csys(origin, mx, &wcs));  
        UF_CALL(UF_CSYS_set_wcs(wcs));  
        return orig_wcs;  
    }

```

#### 代码解析

> 这段代码定义了一个名为set_wcs_by_face_miduv的函数，用于根据NX模型的指定面(face)的中点UV坐标，设置一个新的工作坐标系(wcs)。
>
> 主要步骤包括：
>
> 1. 获取当前的工作坐标系(orig_wcs)。
> 2. 获取指定面的最小和最大UV坐标，并计算中点坐标。
> 3. 根据中点坐标和面的方向，初始化一个3x3的转换矩阵(csys)。
> 4. 使用该矩阵创建一个临时坐标系(mx)。
> 5. 以面的中点为原点，创建一个新的临时工作坐标系(wcs)。
> 6. 设置新的工作坐标系(wcs)为当前坐标系。
> 7. 返回原始的工作坐标系(orig_wcs)。
>
> 通过这些步骤，代码实现了根据指定面的中点UV坐标，动态创建并切换到新的工作坐标系，以便进行后续的几何操作。
>
