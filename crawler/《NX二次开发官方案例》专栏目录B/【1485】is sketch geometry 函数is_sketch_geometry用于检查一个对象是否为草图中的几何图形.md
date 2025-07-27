### 【1485】is sketch geometry 函数is_sketch_geometry用于检查一个对象是否为草图中的几何图形

#### 代码

```cpp
    static tag_t ask_next_sketch(tag_t part, tag_t sketch)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_sketch_type, &sketch));  
        return (sketch);  
    }  
    static logical is_sketch_geometry(tag_t object)  
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
        return is_sketch_geom;  
    }

```

#### 代码解析

> 这段代码的主要功能是遍历NX部件中的所有草图，并检查指定的对象是否属于某个草图。
>
> 具体步骤包括：
>
> 1. 定义一个函数ask_next_sketch，用于遍历部件中的所有草图。它接收当前草图和部件的tag，并返回下一个草图的tag。
> 2. 定义一个函数is_sketch_geometry，用于判断指定对象是否属于某个草图。它首先获取当前显示的部件，然后遍历该部件中的所有草图。对于每个草图，它获取草图中的所有几何对象，并检查指定对象是否在列表中。如果在某个草图中发现指定对象，则返回TRUE。
> 3. is_sketch_geometry函数通过循环调用ask_next_sketch来遍历所有草图，直到找到包含指定对象的草图或者遍历完所有草图为止。
> 4. 该代码使用NX提供的UF接口来实现遍历草图和获取草图几何对象的功能。
> 5. 该代码可以用于判断一个对象是否属于NX草图中的几何对象，从而进行相应的处理，例如在草图编辑模式下进行操作。
>
> 这段代码的主要作用是为NX二次开发提供了一种检查对象是否属于草图的机制，具有实用价值。
>
