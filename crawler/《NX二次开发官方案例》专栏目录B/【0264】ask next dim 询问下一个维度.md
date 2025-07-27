### 【0264】ask next dim 询问下一个维度

#### 代码

```cpp
    static tag_t ask_next_dim(tag_t part, tag_t dim)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_dimension_type, &dim))  
            && (dim != NULL_TAG))  
        {  
            if (!UF_ASSEM_is_occurrence(dim)) return dim;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段NX Open C++代码的主要功能是遍历指定部件(part)的某个维度(dim)的所有子对象，并返回下一个未被使用的维度标签。
>
> 关键点：
>
> 1. 使用UF_OBJ_cycle_objs_in_part函数遍历part的dim类型子对象。
> 2. 使用while循环不断获取下一个dim，直到返回NULL_TAG表示没有更多子对象。
> 3. 使用UF_ASSEM_is_occurrence判断dim是否已被使用，如果未被使用则返回dim。
> 4. 如果dim已被使用，则循环继续获取下一个dim。
> 5. 最终返回第一个未被使用的dim，如果没有则返回NULL_TAG。
>
> 整体来看，这段代码实现了NX部件维度标签的遍历和筛选功能，用于在NX Open中获取可用的维度标签。
>
