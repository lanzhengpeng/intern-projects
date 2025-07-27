### 【0285】ask next of name 问下一个名字

#### 代码

```cpp
    static tag_t ask_next_of_name(char *name, tag_t object)  
    {  
        UF_CALL(UF_OBJ_cycle_by_name(name, &object));  
        return object;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API中用于NX 12.0的二次开发示例代码，其主要功能是调用NX的UF_OBJ_cycle_by_name函数，遍历NX模型中的对象，直到找到名称与参数name匹配的对象，然后返回该对象的tag。代码的具体介绍如下：
>
> 1. 函数名为ask_next_of_name，表示用于查询下一个名称匹配的对象。
> 2. 参数char *name表示要查询的对象名称。
> 3. 参数tag_t object表示当前遍历到的对象，初始调用时传入任意对象即可。
> 4. UF_CALL宏用于调用NX的UF_OBJ_cycle_by_name函数，并处理错误。
> 5. UF_OBJ_cycle_by_name函数用于根据名称遍历NX模型中的对象，如果找到名称匹配的对象，则返回该对象的tag。
> 6. 函数返回找到的名称匹配的对象的tag，如果没有找到则返回空。
> 7. 该函数可用于根据名称查询NX模型中的对象，例如查询特定名称的层、组、特征等。
> 8. 需要注意，该函数仅遍历当前会话中的对象，不会遍历加载的其他NX文件中的对象。
> 9. 使用该函数时，需确保传入的名称与NX模型中对象的名称完全匹配，区分大小写。
> 10. 该函数可用于开发NX的二次开发应用程序，实现对象查询和管理等功能。
>
