### 【2821】set wcs by face minuv 根据面的最小UV值设置WCS

#### 代码

```cpp
    static tag_t set_wcs_by_face_minuv(tag_t face)  
    {  
        tag_t  
            wcs,  
            mx;  
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
        parm[0] = uvs[0];  
        parm[1] = uvs[2];  
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

> 这段代码是NX二次开发中用于设置工作坐标系(WCS)的函数。具体步骤如下：
>
> 1. 获取原始的WCS。
> 2. 获取指定面片的UV参数范围。
> 3. 获取面片的属性，包括原点、X轴方向、Y轴方向。
> 4. 使用面片的X轴和Y轴方向初始化一个3x3的旋转矩阵。
> 5. 根据旋转矩阵创建一个临时坐标系。
> 6. 根据原点和旋转矩阵创建一个新的WCS。
> 7. 设置当前WCS为新创建的WCS。
> 8. 返回原始的WCS。
>
> 总的来说，这段代码的目的是根据指定面片的属性，创建一个新的WCS，并将其设置为当前WCS，同时保留了原始WCS以便后续恢复。
>
