### 【0682】create duplicate alive normal csys 创建一个活跃的正态坐标系副本

#### 代码

```cpp
    static tag_t create_duplicate_alive_normal_csys(tag_t csys)  
    {  
        tag_t  
            matrix,  
            new_csys;  
        double  
            origin[3];  
        UF_CALL(UF_CSYS_ask_csys_info(csys, &matrix, origin));  
        UF_CALL(UF_CSYS_create_csys(origin, matrix, &new_csys));  
        return new_csys;  
    }

```

#### 代码解析

> 这段代码的功能是创建一个与原始坐标系 csys 完全相同的坐标系。
>
> 代码步骤如下：
>
> 1. 获取原始坐标系 csys 的信息，包括矩阵 matrix 和原点 origin。
> 2. 使用原始坐标系的原点 origin 和矩阵 matrix，通过 UF_CSYS_create_csys API 创建一个全新的坐标系 new_csys。
> 3. 返回新创建的坐标系 new_csys。
>
> 通过复制原始坐标系的信息来创建新坐标系，实现了对原始坐标系的无缝复制。
>
