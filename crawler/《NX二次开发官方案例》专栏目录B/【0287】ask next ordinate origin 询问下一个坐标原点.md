### 【0287】ask next ordinate origin 询问下一个坐标原点

#### 代码

```cpp
    static tag_t ask_next_ordinate_origin(tag_t part, tag_t item)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_dimension_type, &item))  
                && (item != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(item, &type, &subtype));  
            if (subtype == UF_dim_ordinate_origin_subtype) return item;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX二次开发中的函数，其功能是在指定的零件中查找下一个坐标原点对象。具体来说，代码的主要功能包括：
>
> 1. 定义了一个名为ask_next_ordinate_origin的静态函数，接受两个参数part和item。part代表零件对象，item代表要查找的起始对象。
> 2. 在零件对象part中循环遍历所有对象，直到找到下一个坐标原点对象。循环过程中，首先调用UF_OBJ_cycle_objs_in_part函数，将item设置为下一个对象，并检查item是否为空。
> 3. 每次循环中，调用UF_OBJ_ask_type_and_subtype函数获取item的类型和子类型，并检查子类型是否为UF_dim_ordinate_origin_subtype（坐标原点）。
> 4. 如果找到符合条件的目标对象，则返回该对象。如果在遍历完所有对象后仍未找到，则返回NULL_TAG。
> 5. 该函数主要用于在NX零件中快速查找坐标原点对象，可以避免手动遍历所有对象，提高开发效率。
>
> 总的来说，这段代码通过遍历零件对象，并检查每个对象的子类型，实现了快速查找坐标原点对象的功能。
>
