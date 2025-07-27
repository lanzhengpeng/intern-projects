### 【1727】paste objects into part 将对象粘贴到零件中

#### 代码

```cpp
    static void paste_objects_into_part(int n_objects, tag_t *objects, tag_t part)  
    {  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_export_options_t  
            options = { FALSE, UF_PART_remove_params, UF_PART_copy_exp_shallowly };  
        UF_CALL(UF_PART_ask_part_name(part, part_name));  
        UF_CALL(UF_PART_export_with_options(part_name, n_objects, objects, &options));  
    }

```

#### 代码解析

> 这段代码是用于将对象粘贴到NX部件中的二次开发函数。
>
> 主要步骤包括：
>
> 1. 定义了函数paste_objects_into_part，用于将指定数量的对象粘贴到指定的部件中。
> 2. 在函数内部，定义了部件名称的字符数组part_name，用于存储要粘贴到的部件的名称。
> 3. 定义了部件导出选项结构体options，其中包括了是否移除参数、移除参数时的处理方式以及是否浅拷贝表达式等选项。
> 4. 调用UF_PART_ask_part_name函数，获取要粘贴到的部件的名称，并存储在part_name数组中。
> 5. 调用UF_PART_export_with_options函数，根据指定的选项，将对象数组objects中的对象粘贴到名为part_name的部件中。
> 6. 整个函数以静态方式声明，这意味着它只能在当前文件中使用。
>
> 总的来说，这个函数实现了将对象数组粘贴到指定部件的功能，并通过选项结构体控制了粘贴过程中的参数和表达式处理方式。
>
