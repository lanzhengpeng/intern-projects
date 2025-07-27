### 【2842】set wcs to sketch 设置草图坐标系

#### 代码

```cpp
    static tag_t set_wcs_to_sketch(tag_t sketch)  
    {  
        double  
            csys[9];  
        tag_t  
            mx,  
            orig_wcs,  
            wcs;  
        UF_SKET_info_t  
            sket_info;  
        UF_CALL(UF_CSYS_ask_wcs(&orig_wcs));  
        UF_CALL(UF_SKET_ask_sketch_info(sketch, &sket_info));  
        UF_CALL(UF_MTX3_initialize(sket_info.csys, &sket_info.csys[3], csys));  
        UF_CALL(UF_CSYS_create_matrix(csys, &mx));  
        UF_CALL(UF_CSYS_create_temp_csys(&sket_info.csys[9], mx, &wcs));  
        UF_CALL(UF_CSYS_set_wcs(wcs));  
        return orig_wcs;  
    }

```

#### 代码解析

> 这段代码是NX二次开发代码，主要功能是将草图坐标系设置为当前工作坐标系。具体步骤如下：
>
> 1. 获取原始的工作坐标系(原始wcs)
> 2. 获取草图信息，包括草图坐标系(sketch csys)
> 3. 根据草图坐标系信息，初始化一个3x3的矩阵，用于创建坐标系
> 4. 使用初始化的矩阵，创建一个新的坐标系(mx)
> 5. 以草图坐标系为参考，创建一个临时坐标系(wcs)
> 6. 将临时坐标系设置为当前工作坐标系
> 7. 返回原始的工作坐标系
>
> 通过设置草图坐标系为工作坐标系，可以方便地在草图平面上进行操作。在完成操作后，返回原始的工作坐标系，以确保不会影响其他坐标系。
>
