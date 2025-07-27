### 【1484】is sketch dimension 函数is_sketch_dimension用于判断给定对象是否为草图中的尺寸，其逻辑如下：1. 遍历当前显示部件中的所有草图2. 对于每个草图，获

#### 代码

```cpp
    static tag_t ask_next_sketch(tag_t part, tag_t sketch)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_sketch_type, &sketch));  
        return (sketch);  
    }  
    static logical is_sketch_dimension(tag_t object)  
    {  
        logical  
            is_sketch_dim = FALSE;  
        int  
            ii,  
            n;  
        tag_t  
            *dims,  
            part = UF_PART_ask_display_part(),  
            sketch = NULL_TAG;  
        while (!is_sketch_dim &&  
               ((sketch = ask_next_sketch(part, sketch)) != NULL_TAG))  
        {  
            UF_CALL(UF_SKET_ask_dimensions_of_sketch(sketch, &n, &dims));  
            for (ii = 0; ii < n; ii++)  
                if (dims[ii] == object)  
                {  
                    is_sketch_dim = TRUE;  
                    break;  
                }  
            if (n > 0) UF_free(dims);  
        }  
        return is_sketch_dim;  
    }

```

#### 代码解析

> 这段NX Open C++代码实现了以下功能：
>
> 1. ask_next_sketch()函数用于遍历当前部件中的所有草图，并返回下一个草图对象tag。它接受当前草图tag作为参数，并循环调用UF_OBJ_cycle_objs_in_part()函数来获取下一个草图。
> 2. is_sketch_dimension()函数用于判断一个对象是否为草图中的尺寸对象。它首先获取当前部件和第一个草图，然后遍历所有草图，获取每个草图的尺寸数组，并检查指定对象是否存在于这些尺寸数组中。如果找到匹配，则返回true，表示该对象是草图尺寸；否则返回false。
> 3. 该代码使用了NX Open API中的UF_OBJ和UF_SKET接口函数，用于遍历草图和查询草图尺寸。
> 4. 在is_sketch_dimension()函数中，使用了循环和条件判断语句来遍历所有草图并检查每个草图中的尺寸。
> 5. 注意，在释放尺寸数组时，先检查数组长度是否大于0，以避免释放空指针导致崩溃。
>
> 总体来说，这段代码实现了在NX部件中查询指定对象是否为草图尺寸的功能。
>
