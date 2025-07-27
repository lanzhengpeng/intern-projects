### 【0294】ask next sketch 询问下一个草图

#### 代码

```cpp
    static tag_t ask_next_sketch(tag_t part, tag_t sketch)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_sketch_type, &sketch));  
        return (sketch);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API的示例，用于在NX部件中遍历草图。
>
> 代码的主要功能如下：
>
> 1. 函数ask_next_sketch接收两个参数：part表示NX部件对象，sketch表示当前的草图对象。
> 2. UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_sketch_type, &sketch));这行代码使用NX Open C++ API的UF_OBJ_cycle_objs_in_part函数，在指定的部件内遍历草图对象。UF_sketch_type指定了要遍历的对象类型为草图。&sketch是一个输出参数，用于接收下一个草图对象。
> 3. 函数返回下一个草图对象。如果没有下一个草图，则sketch将被设置为NULL。
> 4. 该函数可以用于遍历部件中的所有草图，每次调用都会返回下一个草图对象，直到没有草图对象为止。
> 5. UF_CALL是一个宏，用于检查函数调用是否成功，如果不成功会抛出异常。
>
> 总的来说，这段代码实现了在NX部件中按顺序遍历草图的功能，每次调用都会返回下一个草图对象，直到遍历结束。
>
