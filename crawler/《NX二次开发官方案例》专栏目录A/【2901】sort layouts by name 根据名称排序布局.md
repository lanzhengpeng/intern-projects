### 【2901】sort layouts by name 根据名称排序布局

#### 代码

```cpp
    static void sort_layouts_by_name(int n, tag_t *layouts)  
    {  
        int  
            ii;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            layout;  
        char  
            *layout_names;  
        UF_CALL(allocate_memory(n * sizeof(char) * (MAX_ENTITY_NAME_SIZE+1),  
            (void **)&layout_names));  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_OBJ_ask_name(layouts[ii],  
                layout_names+(ii*(MAX_ENTITY_NAME_SIZE+1))));  
        qsort(layout_names, n, MAX_ENTITY_NAME_SIZE+1, strcmp);  
        for (ii = 0; ii < n; ii++)  
        {  
            layout = NULL_TAG;  
            UF_CALL(UF_OBJ_cycle_by_name_and_type(part,  
                layout_names+(ii*(MAX_ENTITY_NAME_SIZE+1)), UF_layout_type, FALSE, &layout));  
            layouts[ii] = layout;  
        }  
        UF_free(layout_names);  
    }

```

#### 代码解析

> 这段代码是NX二次开发中的C++函数，其主要功能是根据布局名称对布局进行排序。
>
> 主要步骤包括：
>
> 1. 获取当前显示的部件。
> 2. 为每个布局分配内存，并调用UF_OBJ_ask_name函数获取所有布局的名称。
> 3. 使用qsort函数和strcmp函数对布局名称进行字典排序。
> 4. 遍历排序后的布局名称数组，调用UF_OBJ_cycle_by_name_and_type函数根据名称和类型查找对应的布局实体，并将排序后的布局实体保存到传入的数组中。
> 5. 释放布局名称数组内存。
>
> 该函数实现了根据布局名称对布局进行排序的功能，使得布局顺序更加直观和易用。
>
