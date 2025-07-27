### 【0281】ask next new sketch 查询下一个新草图

#### 代码

```cpp
    static tag_t ask_next_new_sketch(tag_t part, tag_t sketch)  
    {  
        int  
            subtype,  
            type;  
        while  
            (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_sketch_type, &sketch)) &&  
            (sketch != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(sketch, &type, &subtype));  
            if (subtype == UF_v13_sketch_subtype) return (sketch);  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是NX二次开发中用于在当前部件中遍历草图并找出下一个新版本草图的功能。
>
> 主要功能包括：
>
> 1. 通过UF_OBJ_cycle_objs_in_part函数遍历当前部件中的所有草图对象，直到找到下一个新版本草图。
> 2. 在遍历过程中，使用UF_OBJ_ask_type_and_subtype函数获取每个草图的类型和子类型。
> 3. 通过判断草图的子类型是否为新版本草图（UF_v13_sketch_subtype），来找到下一个新版本草图。
> 4. 如果找到新版本草图，则返回该草图tag，否则返回NULL_TAG表示没有找到。
> 5. 使用while循环不断遍历直到找到下一个新版本草图。
> 6. 在遍历过程中，通过UF_OBJ_cycle_objs_in_part函数返回的布尔值来判断是否需要继续遍历。
> 7. 如果找到新版本草图，则退出循环并返回该草图。
>
> 总体来说，这段代码实现了在当前部件中查找下一个新版本草图的功能，通过遍历草图并判断其子类型来实现。
>
