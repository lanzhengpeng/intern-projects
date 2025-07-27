### 【0225】ask first empty layer 询问首个空层

#### 代码

```cpp
    static int ask_first_empty_layer(void)  
    {  
        tag_t  
            obj;  
        int  
            layer;  
        for (layer = 1; layer <= UF_LAYER_MAX_LAYER; layer++)  
        {  
            obj = NULL_TAG;  
            UF_CALL(UF_LAYER_cycle_by_layer( layer, &obj));  
            if(obj == NULL_TAG)  
            {   
                return layer;  
            }  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是用于NX Open C++ API中查询第一个空层的函数。主要功能包括：
>
> 1. 定义一个静态函数ask_first_empty_layer，用于查询第一个空层。
> 2. 使用for循环遍历所有层，层号从1到UF_LAYER_MAX_LAYER。
> 3. 在循环中，首先将obj设置为NULL_TAG，表示没有找到对象。
> 4. 调用UF_CALL(UF_LAYER_cycle_by_layer(layer, &obj))遍历当前层，并将找到的第一个对象指针赋值给obj。
> 5. 如果obj仍然是NULL_TAG，表示当前层为空，则返回当前层号layer。
> 6. 如果遍历完所有层都没有找到空层，则返回0。
> 7. 通过调用UF层遍历函数，实现了按层遍历查找第一个空层的目的。
> 8. 函数使用静态，表示不需要创建实例即可调用。
>
> 总的来说，这段代码实现了NX模型中查询第一个空层号的功能，为后续操作提供了方便。
>
