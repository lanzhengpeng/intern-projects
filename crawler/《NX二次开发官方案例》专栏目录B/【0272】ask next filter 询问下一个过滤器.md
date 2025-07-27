### 【0272】ask next filter 询问下一个过滤器

#### 代码

```cpp
    static tag_t ask_next_filter(tag_t part, tag_t filter)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_filter_type, &filter));  
        return (filter);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API中用于在NX部件中循环遍历过滤器的功能。
>
> 代码中的关键函数是UF_OBJ_cycle_objs_in_part，用于在指定的NX部件part中遍历所有指定类型为UF_filter_type的对象，并返回下一个过滤器的tag。
>
> 具体步骤如下：
>
> 1. 函数ask_next_filter接受两个参数：part和filter，分别表示NX部件的tag和当前过滤器的tag。
> 2. 调用UF_OBJ_cycle_objs_in_part函数，传入部件tag和过滤器类型，并传入当前过滤器tag的地址作为输出参数。
> 3. 函数遍历部件中的所有过滤器对象，并将下一个过滤器的tag存入filter参数指向的地址。
> 4. 返回filter参数，即下一个过滤器的tag。
> 5. 通过循环调用ask_next_filter函数，可以逐个获取部件中的所有过滤器对象。
>
> 这段代码的作用是提供一种遍历部件中所有过滤器的便捷方法，使得开发者能够轻松地获取和操作NX部件中的过滤器对象。
>
