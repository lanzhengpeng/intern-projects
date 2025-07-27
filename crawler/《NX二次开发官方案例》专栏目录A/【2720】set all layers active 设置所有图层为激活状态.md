### 【2720】set all layers active 设置所有图层为激活状态

#### 代码

```cpp
    static void set_all_layers_active(void)  
    {  
        int ii;  
        for (ii = 1; ii < 257; ii++)  
            UF_CALL(UF_LAYER_set_status(ii, UF_LAYER_ACTIVE_LAYER));  
    }

```

#### 代码解析

> 这段代码是用于NX Open的二次开发代码，其主要功能是将NX中的所有图层（Layer）设为激活状态。
>
> 代码通过一个循环，将图层号从1到256依次设为激活状态。其中，UF_CALL宏用于调用NX的UF函数，UF_LAYER_set_status函数用于设置图层的激活状态，参数UF_LAYER_ACTIVE_LAYER表示激活状态。
>
> 具体来说：
>
> 1. 定义了一个静态函数set_all_layers_active，用于设置所有图层的激活状态。
> 2. 在函数内部，通过一个循环遍历所有图层号（1到256）。
> 3. 使用UF_CALL宏调用UF_LAYER_set_status函数，将每个图层的激活状态设置为UF_LAYER_ACTIVE_LAYER。
> 4. 循环结束后，所有图层都被设置为激活状态。
>
> 这段代码的目的是为了在二次开发中方便地激活所有图层，以便进行后续的图形操作。
>
