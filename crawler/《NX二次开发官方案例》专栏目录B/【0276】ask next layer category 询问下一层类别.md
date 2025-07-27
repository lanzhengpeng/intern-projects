### 【0276】ask next layer category 询问下一层类别

#### 代码

```cpp
    static tag_t ask_next_layer_category(tag_t part, tag_t cat)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_layer_category_type, &cat));  
        return cat;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API中的示例代码，用于在NX CAD系统中的部件(part)中遍历图层类别(category)。具体来说：
>
> 1. ask_next_layer_category是一个静态函数，用于获取当前部件中下一个图层类别。
> 2. 函数接受两个参数：part表示当前部件，cat表示当前的图层类别。
> 3. UF_CALL宏用于调用NX Open C++ API函数。这里调用的是UF_OBJ_cycle_objs_in_part函数，用于遍历部件中的对象。
> 4. UF_OBJ_cycle_objs_in_part函数的第一个参数是部件，第二个参数是对象类型，这里是UF_layer_category_type表示图层类别，第三个参数是输出参数，用于接收下一个图层类别对象。
> 5. 函数返回下一个图层类别对象。
> 6. 这个函数的作用是在部件中遍历所有图层类别，每次调用返回下一个图层类别，如果遍历结束则返回NULL。
> 7. 示例代码展示了如何在NX部件中遍历图层类别，获取每个图层类别的信息，进而进行后续处理。
>
