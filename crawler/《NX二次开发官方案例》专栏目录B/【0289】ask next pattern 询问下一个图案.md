### 【0289】ask next pattern 询问下一个图案

#### 代码

```cpp
    static tag_t ask_next_pattern(tag_t part, tag_t object)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_pattern_type, &object));  
        return (object);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API实现的，用于查询NX部件中的下一个模式。
>
> 主要功能如下：
>
> 1. 定义函数ask_next_pattern，接受两个参数part和object，分别表示NX部件和模式对象。
> 2. 调用UF_OBJ_cycle_objs_in_partAPI函数，遍历部件中的所有模式对象。其中UF_pattern_type表示模式对象类型，&object表示模式对象。
> 3. 返回下一个模式对象，如果部件中没有更多模式对象，则返回NULL。
> 4. 使用UF_CALL宏，用于封装API调用，处理可能的异常。
> 5. 函数为静态函数，属于类内部函数，外部无法直接调用。
> 6. 返回值类型为tag_t，表示NX对象标识符。
> 7. 函数名称以ask开头，表示这是一个查询函数。
> 8. 函数功能单一，只负责查询下一个模式对象。
>
> 综上所述，这段代码实现了在NX部件中查询下一个模式对象的功能，通过遍历部件中的模式对象并返回下一个模式对象。
>
