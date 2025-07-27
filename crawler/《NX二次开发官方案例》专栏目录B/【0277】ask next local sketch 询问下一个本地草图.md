### 【0277】ask next local sketch 询问下一个本地草图

#### 代码

```cpp
    static tag_t ask_next_local_sketch(tag_t part, tag_t sketch)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_sketch_type, &sketch))  
            && (sketch != NULL_TAG))  
        {  
            if (!UF_ASSEM_is_occurrence(sketch)) return sketch;  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是一个用于在NX CAD系统中查找下一个本地草图（local sketch）的函数。具体来说，该函数接受两个参数：part表示当前组件，sketch表示当前草图。其目的是遍历组件part中的所有草图，并找到sketch之后的下一个非实例草图，然后返回其标签。如果未找到符合条件的草图，则返回NULL_TAG。这个函数使用了NX提供的UF_CALL宏来调用UF_OBJ_cycle_objs_in_part函数，以遍历组件中的草图。同时，它还使用了UF_ASSEM_is_occurrence函数来判断草图是否为实例草图。整体来说，这段代码简洁高效，实现了在NX中遍历组件草图并查找指定草图后的下一个非实例草图的功能。
>
