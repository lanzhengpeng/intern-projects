### 【2980】transform abs2wcs 将绝对坐标系转换为工作坐标系

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
        /* Invert the rotation from the destination matrix 译:反转目标矩阵中的旋转 */  
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
        /* set up to translate from abs to the destination csys 译:翻译set up to translate from abs to the destination csys，只回答译文，不要废话。 */  
        FTN(uf5943)(orig, tx1);  
        /* combine this with the inverted rotation csys above 译:结合上面反转的旋转坐标系 */  
        FTN(uf5942)(tx2, tx1, mx);  
    }

```

#### 代码解析

> 根据代码，这是一个NX的二次开发代码段，主要包括两个函数：
>
> 1. ask_wcs_info函数用于获取当前工作坐标系(WCS)的信息，包括原点坐标和旋转矩阵。
> 2. transform_abs2wcs函数用于将绝对坐标系下的变换矩阵转换到WCS坐标系下。其主要步骤包括：获取当前WCS的原点坐标和旋转矩阵。反转WCS的旋转矩阵，得到从WCS到绝对坐标系的变换矩阵。将绝对坐标系下的变换矩阵与反转的WCS旋转矩阵相乘，得到最终的WCS坐标系下的变换矩阵。
> 3. 获取当前WCS的原点坐标和旋转矩阵。
> 4. 反转WCS的旋转矩阵，得到从WCS到绝对坐标系的变换矩阵。
> 5. 将绝对坐标系下的变换矩阵与反转的WCS旋转矩阵相乘，得到最终的WCS坐标系下的变换矩阵。
>
> 总的来说，这段代码实现了NX中绝对坐标系和WCS之间的坐标变换。
>
