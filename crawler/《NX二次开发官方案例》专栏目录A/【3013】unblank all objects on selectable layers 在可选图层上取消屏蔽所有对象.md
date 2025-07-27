### 【3013】unblank all objects on selectable layers 在可选图层上取消屏蔽所有对象

#### 代码

```cpp
    static void unblank_all_objects_on_selectable_layers(void)  
    {  
        tag_t  
            object = NULL_TAG;  
        while (!UF_CALL(UF_LAYER_cycle_by_layer(0, &object)) &&  
            (object != NULL_TAG))  
            UF_CALL(UF_OBJ_set_blank_status(object, UF_OBJ_NOT_BLANKED));  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能是取消所有可选图层上对象的可视化隐藏状态。具体来说：
>
> 1. 首先，通过UF_CALL(UF_LAYER_cycle_by_layer(0, &object))遍历所有可选图层，并将每个图层上的对象赋值给变量object。
> 2. 然后通过一个while循环，当object不为NULL_TAG时，循环执行以下操作：调用UF_OBJ_set_blank_status(object, UF_OBJ_NOT_BLANKED)来取消当前对象的可视化隐藏状态。
> 3. 调用UF_OBJ_set_blank_status(object, UF_OBJ_NOT_BLANKED)来取消当前对象的可视化隐藏状态。
> 4. 通过UF_CALL宏来调用NX Open API函数，以处理错误。
> 5. 在遍历完所有可选图层上的对象后，所有对象的可视化隐藏状态都会被取消。
> 6. 这个函数的目的是为了在NX中使原本隐藏的对象可见，以便进行后续的二次开发操作。
> 7. 注意，这个函数仅取消对象的可视化隐藏状态，不会改变其图层属性或是否选中状态。
>
> 总的来说，这段代码实现了遍历可选图层上的所有对象，并取消其可视化隐藏状态的功能，为后续的二次开发操作提供了便利。
>
