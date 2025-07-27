### 【0266】ask next drawing 询问下一个绘图

#### 代码

```cpp
    static tag_t ask_next_drawing(tag_t part, tag_t drawing)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drawing_type, &drawing));  
        return (drawing);  
    }

```

#### 代码解析

> 这段代码是用于NX Open API的二次开发，其主要功能是在NX部件中遍历所有图样，并返回下一个图样对象。
>
> 代码的关键点包括：
>
> 1. 函数名：ask_next_drawing，表明该函数的功能是获取下一个图样。
> 2. 参数：part是部件对象，drawing是图样对象，用于指定遍历的起始图样。
> 3. UF_OBJ_cycle_objs_in_part：使用该函数在部件内遍历所有图样，并返回下一个图样对象。
> 4. 返回值：函数返回下一个图样对象。
> 5. UF_drawing_type：指定遍历类型为图样。
> 6. UF_CALL：使用该宏来调用NX Open API函数，可以简化错误处理。
>
> 总的来说，这段代码通过在部件内遍历图样，实现了获取下一个图样的功能，为NX的二次开发提供了方便。
>
