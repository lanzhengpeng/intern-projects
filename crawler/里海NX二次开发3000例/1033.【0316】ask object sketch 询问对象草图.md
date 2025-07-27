### 【0316】ask object sketch 询问对象草图

#### 代码

```cpp
    static tag_t ask_next_sketch(tag_t part, tag_t sketch)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_sketch_type, &sketch));  
        return (sketch);  
    }  
    static tag_t ask_object_sketch(tag_t object)  
    {  
        logical  
            is_sketch_geom = FALSE;  
        int  
            ii,  
            n;  
        tag_t  
            *geoms,  
            part = UF_PART_ask_display_part(),  
            sketch = NULL_TAG;  
        while (!is_sketch_geom &&  
               ((sketch = ask_next_sketch(part, sketch)) != NULL_TAG))  
        {  
            UF_CALL(UF_SKET_ask_geoms_of_sketch(sketch, &n, &geoms));  
            for (ii = 0; ii < n; ii++)  
                if (geoms[ii] == object)  
                {  
                    is_sketch_geom = TRUE;  
                    break;  
                }  
            if (n > 0) UF_free(geoms);  
        }  
        return sketch;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API实现的，其主要功能是在NX中查询给定对象所属的草图。
>
> 代码主要包含两个函数：
>
> 1. ask_next_sketch(part, sketch)：这个函数用于遍历指定部件中的所有草图。它会接收当前部件的tag和上一个草图的tag，返回下一个草图的tag。如果当前部件中已经没有草图，则返回NULL_TAG。
> 2. ask_object_sketch(object)：这个函数用于查询给定对象所属的草图。它会首先获取当前显示的部件，然后遍历该部件中的所有草图，判断每个草图是否包含给定对象。如果找到包含该对象的草图，则返回该草图的tag。
>
> 通过这两个函数的配合，我们可以实现查询NX中任意对象所属草图的功能。这种功能在二次开发中非常有用，比如在创建特征时需要获取草图作为输入，就可以使用这个函数来获取。
>
