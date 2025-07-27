### 【2829】set wcs by planar face 根据平面面设置wcs

#### 代码

```cpp
    static tag_t set_wcs_by_planar_face(tag_t face)  
    {  
        tag_t  
            wcs,  
            orig_wcs,  
            mx;  
        double  
            csys[9],  
            junk[3],  
            origin[3],  
            parm[2],  
            uvs[4];  
        UF_CALL(UF_CSYS_ask_wcs(&orig_wcs));  
        UF_CALL(UF_MODL_ask_face_uv_minmax(face, uvs));  
        parm[0] = (uvs[0] + uvs[1]) / 2;  
        parm[1] = (uvs[2] + uvs[3]) / 2;  
        UF_CALL(UF_MODL_ask_face_props(face, parm, origin, &csys[0], &csys[3],  
            junk, junk, &csys[6], junk));  
        UF_CALL(UF_CSYS_ask_matrix_of_object(face, &mx));  
        UF_CALL(UF_CSYS_create_temp_csys(origin, mx, &wcs));  
        UF_CALL(UF_CSYS_set_wcs(wcs));  
        return orig_wcs;  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发的，其主要功能是设置工作坐标系(wcs)。
>
> 代码流程如下：
>
> 1. 获取当前的工作坐标系(orig_wcs)。
> 2. 获取指定面的UV参数范围(uvs)。
> 3. 计算指定面的中心UV参数值。
> 4. 获取指定面的法线、原点和方向余弦矩阵。
> 5. 获取指定面所在物体的变换矩阵(mx)。
> 6. 根据面的中心点、法线和变换矩阵创建一个临时的坐标系(wcs)。
> 7. 设置临时坐标系为当前的工作坐标系。
> 8. 返回原始的工作坐标系(orig_wcs)。
>
> 总的来说，这段代码通过获取面的信息，计算面的中心，并基于面的法线和变换矩阵创建一个临时的坐标系，并设置其为当前工作坐标系。这一功能在NX建模和编程中非常常见。
>
