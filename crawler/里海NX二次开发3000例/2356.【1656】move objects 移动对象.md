### 【1656】move objects 移动对象

#### 代码

```cpp
    static void move_objects(int n, tag_t *stuff, double delta[3])  
    {  
        double  
            mx[16];  
        FTN(uf5943)(delta, mx);  
        UF_CALL(UF_MODL_transform_entities(n, stuff, mx));  
        UF_CALL(UF_MODL_update());  
    }

```

#### 代码解析

> 这段代码是NX二次开发代码，用于实现移动物体的功能。其中涉及的主要功能包括：
>
> 1. 函数定义：move_objects是一个静态函数，用于移动物体。
> 2. 参数说明：函数接受三个参数，分别为n（物体数量）、stuff（物体数组）和delta（移动距离数组）。
> 3. 内部实现：创建变换矩阵：调用uf5943函数，根据移动距离delta生成变换矩阵mx。物体变换：使用UF_MODL_transform_entities函数，根据变换矩阵mx对物体数组stuff进行变换。更新模型：调用UF_MODL_update函数更新模型，以反映物体的移动。
> 4. 创建变换矩阵：调用uf5943函数，根据移动距离delta生成变换矩阵mx。
> 5. 物体变换：使用UF_MODL_transform_entities函数，根据变换矩阵mx对物体数组stuff进行变换。
> 6. 更新模型：调用UF_MODL_update函数更新模型，以反映物体的移动。
> 7. 函数调用：UF_CALL宏用于调用NX的UF函数，并处理错误。
>
> 总的来说，这段代码通过变换矩阵实现了NX中物体的移动功能。作者在代码中通过封装实现了物体移动的通用功能，提高了代码的复用性。
>
