### 【0387】ask where used in session 询问会话中的使用位置

#### 代码

```cpp
    static int ask_where_used_in_session(tag_t part, uf_list_p_t *parents)  
    {  
        int  
            n;  
        UF_CALL(UF_MODL_create_list(parents));  
        add_parents_to_list(part, *parents);  
        UF_CALL(UF_MODL_ask_list_count(*parents, &n));  
        return n;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API的一个示例，主要实现了以下功能：
>
> 1. 定义了一个名为ask_where_used_in_session的静态函数，该函数接受一个NX部件的tag(part)和指向父部件列表的指针(parents)。
> 2. 使用UF_MODL_create_list创建一个空的父部件列表。
> 3. 调用add_parents_to_list函数，将指定部件的所有父部件添加到列表中。这个函数需要自己实现。
> 4. 使用UF_MODL_ask_list_count函数获取父部件列表中的父部件数量，并返回这个数量。
> 5. 该函数的目的是查询NX会话中指定部件的父部件数量，常用于查询部件在装配中的使用情况。
> 6. 函数以UF_CALL宏开头，这是NX API调用前的标准写法，用于处理函数调用错误。
> 7. static表示这是一个内部使用的静态函数，不需要暴露给外部。
> 8. 整体来看，这段代码比较简洁，实现了查询部件在NX会话中使用情况的功能，可以作为NX二次开发的一个参考。
>
