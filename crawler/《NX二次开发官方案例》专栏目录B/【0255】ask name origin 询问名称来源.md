### 【0255】ask name origin 询问名称来源

#### 代码

```cpp
    static void ask_name_origin(tag_t object, double *origin)  
    {  
        if (UF_OBJ_ask_name_origin(object, origin))  
        {  
            UF_CALL(UF_OBJ_set_name(object, "X"));  
            UF_CALL(UF_OBJ_ask_name_origin(object, origin));  
            UF_CALL(UF_OBJ_delete_name(object));  
        }  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发的C++函数，其功能是获取和修改NX对象（如特征、实体等）的名称和原点坐标。下面是对代码的详细解释：
>
> 1. 函数名：ask_name_origin，用于获取NX对象的名称和原点坐标。
> 2. 参数：tag_t object表示要操作的NX对象，double *origin用于接收原点坐标。
> 3. 使用UF_OBJ_ask_name_origin函数获取对象的名称和原点坐标。如果成功，则进入下一步。
> 4. 使用UF_CALL(UF_OBJ_set_name(object, "X"))将对象名称设置为"X"。
> 5. 再次调用UF_OBJ_ask_name_origin函数，此时会返回更新后的名称和原点坐标。
> 6. 使用UF_OBJ_delete_name删除对象的名称。
> 7. 整个函数通过UF函数接口与NX进行交互，实现了对NX对象名称和原点的查询、设置和删除操作。
> 8. 使用UF_CALL宏来调用UF函数，它可以简化错误处理，避免手动处理函数返回值。
> 9. 函数遵循NX二次开发的编程规范，使用UF接口进行NX对象操作。
> 10. 通过该函数，可以方便地获取和修改NX对象的名称和原点坐标，为后续的NX开发提供了基础支持。
>
> 总的来说，这段代码实现了基本的NX对象名称和原点坐标的查询、设置和删除功能，为NX二次开发提供了基础支持。
>
