### 【2771】set layer object array 设置层对象数组

#### 代码

```cpp
    static void set_layer_object_array(int n_objs, tag_t *objs, int layer)  
    {  
        logical  
            is_display;  
        int  
            ii;  
        for (ii = 0; ii < n_objs; ii++)  
        {  
            UF_CALL(UF_OBJ_is_displayable(objs[ii], &is_display));  
            if (is_display)  
                UF_CALL(UF_OBJ_set_layer(objs[ii], layer));  
        }  
    }

```

#### 代码解析

> 这段代码是一个用于设置NX对象图层的函数，其主要功能如下：
>
> 1. 函数名和参数：函数名为set_layer_object_array，接受三个参数：n_objs（对象数量）、objs（对象数组）和layer（目标图层编号）。
> 2. 遍历对象：函数通过一个for循环遍历传入的对象数组，对每个对象执行操作。
> 3. 检查对象是否可显示：使用UF_OBJ_is_displayable函数检查每个对象是否可以显示。
> 4. 设置图层：如果对象可以显示，使用UF_OBJ_set_layer函数将该对象设置到传入的图层编号。
> 5. 错误处理：通过UF_CALL宏调用UF函数，实现错误处理。
> 6. 整体功能：该函数的目的是将传入的对象数组中的对象批量设置到一个指定的图层。
> 7. NX二次开发：这段代码属于NX的二次开发代码，利用UF函数实现了NX对象图层的批量设置。
> 8. 应用场景：可以用于将模型中的某些对象批量隐藏到指定的图层中，便于管理复杂的NX模型。
>
> 总之，这段代码提供了一个批量设置NX对象图层的实用函数，通过简单的循环和UF函数调用，实现了对NX模型对象图层的有效管理。
>
