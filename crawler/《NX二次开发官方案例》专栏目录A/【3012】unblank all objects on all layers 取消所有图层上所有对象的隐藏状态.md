### 【3012】unblank all objects on all layers 取消所有图层上所有对象的隐藏状态

#### 代码

```cpp
    static void unblank_all_objects_on_all_layers(void)  
    {  
        int  
            layer;  
        tag_t  
            object = NULL_TAG;  
        for (layer = 1; layer <= 256; layer++)  
            while (!UF_CALL(UF_LAYER_cycle_by_layer(layer, &object)) &&  
                (object != NULL_TAG))  
                UF_CALL(UF_OBJ_set_blank_status(object, UF_OBJ_NOT_BLANKED));  
    }

```

#### 代码解析

> 这段代码是用于在NX中取消所有层上所有对象的隐藏状态的二次开发代码。主要功能包括：
>
> 1. 循环遍历所有层(1-256层)。
> 2. 对于每层，调用UF_LAYER_cycle_by_layer函数来获取该层上的对象，直到该层上没有更多对象为止。
> 3. 对于每个获取到的对象，调用UF_OBJ_set_blank_status函数将其隐藏状态设置为不隐藏(UF_OBJ_NOT_BLANKED)。
> 4. 这样可以取消所有层上所有对象的隐藏状态，使其全部显示出来。
> 5. 使用UF_CALL宏来处理函数调用，避免需要手动处理函数返回值。
> 6. 使用了适当的错误处理机制，以保证代码的健壮性。
>
> 总之，这段代码的主要作用是遍历所有层并取消所有对象的隐藏状态，使其全部显示出来。
>
