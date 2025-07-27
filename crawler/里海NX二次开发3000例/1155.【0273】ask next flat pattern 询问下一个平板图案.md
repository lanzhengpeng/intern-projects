### 【0273】ask next flat pattern 询问下一个平板图案

#### 代码

```cpp
    static tag_t ask_next_flat_pattern(tag_t part, tag_t object)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_flat_pattern_type, &object));  
        return (object);  
    }

```

#### 代码解析

> 这是NX二次开发的代码，用于询问零件中下一个平展开图对象。主要功能如下：
>
> 1. 定义了一个静态函数ask_next_flat_pattern，它接受两个参数part和object。
> 2. part表示当前零件对象，object表示当前平展开图对象。
> 3. 通过UF_OBJ_cycle_objs_in_part函数遍历part中的所有平展开图对象。当object为NULL时，返回第一个平展开图对象；当object不为NULL时，返回object之后的下一个平展开图对象。
> 4. 如果part中没有平展开图对象，返回NULL。
> 5. 函数返回找到的平展开图对象，或者NULL。
> 6. 该函数主要用于遍历和获取零件中的平展开图对象，实现平展开图的遍历和查询功能。
> 7. UF_OBJ_cycle_objs_in_part是NX提供的函数，用于遍历零件中的对象。UF_flat_pattern_type是预定义的常量，表示平展开图类型。
> 8. &object表示object的地址，函数通过地址修改object的值。
>
> 综上所述，该代码定义了一个用于遍历和获取零件中平展开图对象的函数，实现了NX二次开发中的平展开图遍历和查询功能。
>
