### 【0370】ask view axes 询问视图坐标轴

#### 代码

```cpp
    static void ask_view_axes(tag_t view, double *xdir, double *ydir, double *zdir)  
    {  
        double  
            matrix[9];  
        char  
            name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_OBJ_ask_name(view, name));  
        UF_CALL(uc6433(name, matrix));  
        UF_VEC3_copy(&matrix[0], xdir);  
        UF_VEC3_copy(&matrix[3], ydir);  
        UF_VEC3_copy(&matrix[6], zdir);  
    }

```

#### 代码解析

> 这段代码是用于获取NX视图方向的二次开发代码。
>
> 代码逻辑如下：
>
> 1. 定义了一个静态函数ask_view_axes，用于获取指定视图的三个方向向量(xdir, ydir, zdir)。
> 2. 在函数内部，首先定义了9个元素的一维数组matrix，用于存储视图的方向矩阵。
> 3. 使用UF_OBJ_ask_name函数获取视图的名称，并存储在字符数组name中。
> 4. 通过调用自定义的uc6433函数，传入视图名称，获取该视图的方向矩阵，并存储在matrix数组中。
> 5. 使用UF_VEC3_copy函数，将matrix数组中的前3个元素、中间3个元素和最后3个元素分别拷贝到xdir、ydir和zdir指针所指向的向量中。
> 6. 最后，函数返回，此时xdir、ydir和zdir指针指向的向量即为所求的视图的三个方向向量。
>
> 这段代码的主要功能是获取NX视图的坐标系方向，为后续的视图变换等操作提供基础。
>
