### 【1623】map2 construct space 映射2构建空间

#### 代码

```cpp
    static void map2_construct_space(double pt_loc[3])  
    {  
        int  
            irc;  
        double  
            orig[3],  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12] = { 0,0,0,0,0,0,0,0,0,0,0,0},  
            mx[16];  
        tag_t  
            mat_id,  
            wcs_tag;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs_tag));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs_tag, &mat_id, orig));  
        UF_CALL(UF_CSYS_ask_matrix_values(mat_id, &csys[3]));  
        FTN(uf5940)(abs_mx, csys, mx, &irc);  
        FTN(uf5941)(pt_loc, mx);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，用于计算一个点在当前坐标系下的坐标转换到绝对坐标系下的坐标。具体步骤如下：
>
> 1. 获取当前工作坐标系(wcs)的tag。
> 2. 获取当前工作坐标系的材料id和原点坐标。
> 3. 获取当前工作坐标系的旋转矩阵。
> 4. 构造一个表示绝对坐标系的旋转矩阵。
> 5. 调用uf5940函数，将当前工作坐标系的旋转矩阵转换到绝对坐标系下。
> 6. 调用uf5941函数，将输入的点坐标根据转换后的旋转矩阵从当前工作坐标系转换到绝对坐标系下。
> 7. 输出转换后的点坐标。
>
> 该函数通过获取当前工作坐标系的参数，将输入点从当前坐标系转换到绝对坐标系下。这样就可以在绝对坐标系下进行后续的计算和处理。
>
