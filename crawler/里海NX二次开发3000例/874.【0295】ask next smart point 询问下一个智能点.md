### 【0295】ask next smart point 询问下一个智能点

#### 代码

```cpp
    static tag_t ask_next_smart_point(tag_t part, tag_t point)  
    {  
        logical  
            is_so;  
        int  
            subtype,  
            type;  
        /*  
            Note: UF_OBJ_cycle_objs_in_part will not find invisible smart  
            points so UF_OBJ_cycle_all is used here instead  
        */  
        while ((point = UF_OBJ_cycle_all(part, point)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(point, &type, &subtype));  
            if (type == UF_point_type)  
            {  
                UF_CALL(UF_SO_is_so(point, &is_so));  
                if (is_so) return point;  
            }  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段NX Open C++代码的目的是遍历指定部件中的所有点，并返回下一个智能点(Smart Point)。
>
> 主要思路如下：
>
> 1. 使用UF_OBJ_cycle_all函数遍历指定部件中的所有对象。
> 2. 对于每个对象，使用UF_OBJ_ask_type_and_subtype函数检查其类型。
> 3. 如果对象是点(UF_point_type)，使用UF_SO_is_so函数检查它是否是智能点。
> 4. 如果是智能点，则返回该点的标签。
> 5. 如果不是，继续遍历下一个对象。
> 6. 如果遍历完所有对象后没有找到智能点，返回NULL_TAG。
>
> 这个函数的作用是找到部件中的下一个智能点，并返回其标签。需要注意的是，由于NX的UF_OBJ_cycle_objs_in_part函数不会返回不可见的智能点，所以这里使用了UF_OBJ_cycle_all来遍历所有对象，包括不可见的对象。
>
