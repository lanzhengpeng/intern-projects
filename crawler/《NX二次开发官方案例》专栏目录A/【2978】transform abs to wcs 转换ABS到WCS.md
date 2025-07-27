### 【2978】transform abs to wcs 转换ABS到WCS

#### 代码

```cpp
    static void ask_wcs_info(double origin[3], double axes[9])  
    {  
        tag_t  
            wcs,  
            wcs_mx;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs, &wcs_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, axes));  
    }  
    static void transform_abs2wcs(double mx[16])  
    {  
        double  
            csys[9],  
            orig[3],  
            tx1[12],  
            tx2[12];  
        ask_wcs_info(orig, csys);  
        /* Invert the rotation from the destination matrix 译:翻转目标矩阵的旋转。 */  
        tx2[0] = csys[0];  
        tx2[1] = csys[3];  
        tx2[2] = csys[6];  
        tx2[3] = 0;  
        tx2[4] = csys[1];  
        tx2[5] = csys[4];  
        tx2[6] = csys[7];  
        tx2[7] = 0;  
        tx2[8] = csys[2];  
        tx2[9] = csys[5];  
        tx2[10] = csys[8];  
        tx2[11] = 0;  
        /* set up to translate from abs to the destination csys 译:设置从ABS到目标CSYS的转换 */  
        FTN(uf5943)(orig, tx1);  
        /* combine this with the inverted rotation csys above 译:将此与上述倒转旋转坐标系结合。 */  
        FTN(uf5942)(tx2, tx1, mx);  
    }  
    static void transform_abs_to_wcs(int n, tag_t *objects)  
    {  
        int  
            move = 1,  
            work = -1,  
            off = 2,  
            resp;  
        double  
            mx[16];  
        transform_abs2wcs(mx);  
        FTN(uf5947)(mx, objects, &n, &move, &work, &off, NULL, NULL, &resp);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的NX二次开发代码，其主要功能是实现NX坐标系之间的转换。
>
> 1. ask_wcs_info函数：获取当前工作坐标系(WCS)的坐标系信息，包括原点坐标和坐标系矩阵。
> 2. transform_abs2wcs函数：将绝对坐标系转换为WCS。首先，获取WCS的坐标系信息，然后对WCS的坐标系矩阵进行逆转换，得到从ABS到WCS的转换矩阵。最后，将这个转换矩阵与WCS的原点坐标组合，得到最终的转换矩阵。
> 3. transform_abs_to_wcs函数：对给定的NX对象数组进行从ABS到WCS的转换。首先调用transform_abs2wcs函数获取转换矩阵，然后使用NX API函数UF_CSYS_transform对对象数组进行转换。
>
> 总体来说，这段代码实现了NX坐标系之间的转换功能，为NX二次开发提供了坐标系变换的基础支持。
>
