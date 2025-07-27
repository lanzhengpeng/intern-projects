### 【2823】set wcs by face u and normal at point 设置通过面U和点法线设置WCS

#### 代码

```cpp
    static tag_t set_wcs_by_face_u_and_normal_at_point(tag_t face, double origin[3])  
    {  
        tag_t  
            mx,  
            orig_wcs,  
            wcs;  
        double  
            csys[9],  
            junk[3],  
            mag,  
            parm[2],  
            pnt[3],  
            tol,  
            x_dir[3],  
            y_dir[3],  
            z_dir[3];  
        UF_CALL(UF_CSYS_ask_wcs(&orig_wcs));  
        UF_MODL_ask_distance_tolerance(&tol);  
        UF_CALL(UF_MODL_ask_face_parm(face, origin, parm, pnt));  
        UF_CALL(UF_MODL_ask_face_props(face, parm, pnt, x_dir, junk, junk,  
            junk, z_dir, junk));  
        UF_VEC3_cross(z_dir, x_dir, y_dir);  
        UF_CALL(UF_VEC3_unitize(x_dir, tol, &mag, &csys[0]));  
        UF_CALL(UF_VEC3_unitize(y_dir, tol, &mag, &csys[3]));  
        UF_CALL(UF_VEC3_unitize(z_dir, tol, &mag, &csys[6]));  
        UF_MTX3_ortho_normalize(csys);  
        UF_CALL(UF_CSYS_create_matrix(csys, &mx));  
        UF_CALL(UF_CSYS_create_temp_csys(origin, mx, &wcs));  
        UF_CALL(UF_CSYS_set_wcs(wcs));  
        return orig_wcs;  
    }

```

#### 代码解析

> 这段NX Open API代码的主要功能是在给定的面上设置一个工作坐标系(WCS)。具体步骤如下：
>
> 1. 首先获取原始的WCS，用于后续恢复。
> 2. 获取NX默认的距离公差，用于后续计算。
> 3. 根据给定的面(face)和原点(origin)，查询面在该点的参数和空间点。
> 4. 查询面的属性，获取法向矢量和X轴方向矢量。
> 5. 计算Y轴方向矢量，与X轴和法向矢量构成右手坐标系。
> 6. 对X、Y、Z轴进行单位化处理，得到一个正交的坐标系矩阵。
> 7. 创建一个临时坐标系，以原点和计算出的坐标系矩阵为基础。
> 8. 将临时坐标系设置为当前的工作坐标系(WCS)。
> 9. 返回原始的WCS，以便后续恢复。
>
> 总之，这段代码通过给定的面和原点，计算出一个局部坐标系，并设置为当前的工作坐标系，以便进行后续操作。
>
