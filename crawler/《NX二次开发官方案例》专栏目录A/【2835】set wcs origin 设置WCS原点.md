### 【2835】set wcs origin 设置WCS原点

#### 代码

```cpp
    static tag_t set_wcs_origin(double new_origin[3])  
    {  
        tag_t  
            orig_wcs,  
            wcs,  
            wcs_mx;  
        double  
            old_origin[3];  
        UF_CALL(UF_CSYS_ask_wcs(&orig_wcs));  
        UF_CALL(UF_CSYS_ask_csys_info(orig_wcs, &wcs_mx, old_origin));  
        UF_CALL(UF_CSYS_create_temp_csys(new_origin, wcs_mx, &wcs));  
        UF_CALL(UF_CSYS_set_wcs(wcs));  
        return orig_wcs;  
    }

```

#### 代码解析

> 这段代码是用于设置NX CAD系统的世界坐标系原点的函数。其主要功能包括：
>
> 1. 获取当前的世界坐标系标识(orig_wcs)。
> 2. 获取当前世界坐标系的信息，包括坐标系类型(wcs_mx)和原点坐标(old_origin)。
> 3. 根据新的原点坐标(new_origin)和坐标系类型(wcs_mx)创建一个新的坐标系(wcs)。
> 4. 将新的坐标系设置为当前的世界坐标系。
> 5. 返回之前的世界坐标系标识(orig_wcs)，以便在需要时恢复。
>
> 通过调用NX的UF_CSYS接口实现，主要用于NX CAD系统的坐标系管理。此函数在NX二次开发中具有实用性，可用于调整和设置坐标系，以便进行各种CAD操作。
>
