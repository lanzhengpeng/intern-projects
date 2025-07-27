### 【0008】ask aligned or exact bounding box 询问对齐或精确边界框

#### 代码

```cpp
    static logical ask_aligned_or_exact_aligned_bounding_box(  
        tag_t object, tag_t csys, double *llc, double *urc, int which)  
    {  
        int  
            ii,jj;  
        double  
            dir[3][3],  
            dist[3];  
            if (which == 1)  
            {  
                if (UF_MODL_ask_bounding_box_aligned(object, csys, false, llc,  
                    dir, dist )) return false;  
            }  
            else  
            {  
                if (UF_MODL_ask_bounding_box_exact(object, csys, llc, dir, dist ))  
                    return false;  
            }  
            for (ii = 0; ii < 3; ii++)  
            {  
                urc[ii] = llc[ii];  
                for (jj = 0; jj < 3; jj++)  
                    urc[ii] += dir[jj][ii] * dist[jj];  
            }  
            return true;  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能是获取NX对象在指定坐标系下的对齐或精确对齐的边界框。
>
> 具体来说，代码首先定义了一个名为ask_aligned_or_exact_aligned_bounding_box的静态逻辑函数，用于询问指定NX对象在指定坐标系下的对齐或精确对齐的边界框。
>
> 参数说明如下：
>
> 函数内部逻辑如下：
>
> 1. 判断which的值，决定调用UF_MODL_ask_bounding_box_aligned还是UF_MODL_ask_bounding_box_exact来获取边界框的左下角坐标、方向矩阵和距离向量。
> 2. 计算边界框的右上角坐标，即将左下角坐标加上方向矩阵和距离向量的乘积。
> 3. 返回计算结果。
>
> 该函数的作用是在NX中获取对象在指定坐标系下的对齐或精确对齐的边界框，可用于后续的建模和分析工作。
>
