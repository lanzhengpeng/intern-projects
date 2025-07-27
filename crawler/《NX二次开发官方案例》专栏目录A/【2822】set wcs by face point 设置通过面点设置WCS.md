### 【2822】set wcs by face point 设置通过面点设置WCS

#### 代码

```cpp
    static tag_t set_wcs_by_face_point(tag_t face, double origin[3])  
    {  
        tag_t  
            mx,  
            orig_wcs,  
            wcs;  
        double  
            csys[9],  
            junk[3],  
            parm[2],  
            pnt[3],  
            x_dir[3],  
            y_dir[3];  
        UF_CALL(UF_CSYS_ask_wcs(&orig_wcs));  
        UF_CALLvoid(UF_MODL_ask_face_parm(face, origin, parm, pnt));  
        UF_CALL(UF_MODL_ask_face_props(face, parm, pnt, x_dir, y_dir, junk,  
            junk, junk, junk));  
        UF_CALL(UF_MTX3_initialize(x_dir, y_dir, csys));  
        UF_CALL(UF_CSYS_create_matrix(csys, &mx));  
        UF_CALL(UF_CSYS_create_temp_csys(origin, mx, &wcs));  
        UF_CALL(UF_CSYS_set_wcs(wcs));  
        return orig_wcs;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，用于设置一个新的工作坐标系(WCS)。
>
> 代码的主要逻辑如下：
>
> 1. 获取当前的工作坐标系(orig_wcs)。
> 2. 根据给定的面(face)和原点(origin)，获取面的参数和点。
> 3. 计算面的X轴和Y轴方向。
> 4. 根据X轴和Y轴方向，初始化一个3x3的旋转矩阵。
> 5. 根据旋转矩阵创建一个矩阵对象(mx)。
> 6. 使用原点和矩阵创建一个新的坐标系对象(wcs)。
> 7. 设置新的坐标系为当前的工作坐标系。
> 8. 返回原始的工作坐标系。
>
> 这段代码的主要作用是根据一个面的方向和给定的原点，创建一个新的工作坐标系并设置为当前坐标系。这在NX编程中经常用于定位和变换。
>
