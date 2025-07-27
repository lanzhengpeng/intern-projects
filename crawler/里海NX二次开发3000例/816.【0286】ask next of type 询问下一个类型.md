### 【0286】ask next of type 询问下一个类型

#### 代码

```cpp
    static tag_t ask_next_of_type(tag_t part, int type, tag_t object)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object));  
        return (object);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API中的一部分，其主要功能是获取NX部件(part)中下一个指定类型的对象(object)。
>
> 关键点：
>
> 1. ask_next_of_type是一个静态函数，可以不创建对象直接调用。
> 2. 参数part表示NX部件的UF_PART句柄。
> 3. 参数type表示要获取的对象的类型，例如UF_OBJ_CURVE、UF_OBJ_FACE等。
> 4. 参数object表示当前对象句柄，初始值需要传入一个有效的对象句柄，后续调用会返回下一个对象。
> 5. 函数内部调用UF_OBJ_cycle_objs_in_part()函数遍历部件中所有指定类型的对象，并返回下一个对象。
> 6. 返回值是下一个对象的UF_OBJ对象句柄。
>
> 总结：这段代码定义了一个便捷的静态函数，用于在NX部件中按类型遍历获取对象，简化了遍历部件中对象的操作，提高了开发效率。
>
