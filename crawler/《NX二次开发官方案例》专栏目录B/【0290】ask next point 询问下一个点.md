### 【0290】ask next point 询问下一个点

#### 代码

```cpp
    static tag_t ask_next_point(tag_t part, tag_t object)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_point_type, &object));  
        return (object);  
    }

```

#### 代码解析

> 这段代码是用于NX Open C++ API的二次开发代码，其功能是遍历NX部件中的所有点并返回下一个点。
>
> 具体来说：
>
> 1. ask_next_point函数用于遍历NX部件中的所有点，并返回下一个点。
> 2. part参数表示要遍历的NX部件。
> 3. object参数表示当前遍历到的点，在第一次调用时需要传入NULL。
> 4. 使用UF_OBJ_cycle_objs_in_part函数来遍历部件中的点。该函数会遍历所有点，并在每次调用时返回下一个点。
> 5. UF_point_type指定了要遍历的对象类型为点。
> 6. 当object为NULL时，函数开始遍历部件中的第一个点；当object不为NULL时，函数继续从object开始遍历下一个点。
> 7. 遍历结束后，返回NULL表示没有更多点。
> 8. 通过在循环中调用ask_next_point函数，可以遍历部件中的所有点。
>
> 总的来说，这段代码实现了NX部件中所有点的遍历功能，可以方便地在二次开发中获取部件中的所有点。
>
