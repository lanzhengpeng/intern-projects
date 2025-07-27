### 【0540】copy objects 复制对象

#### 代码

```cpp
    static void copy_objects(int n_objects, tag_t *objects)  
    {  
        tag_t  
            part = UF_ASSEM_ask_work_part();  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_export_options_t  
            options = { FALSE, UF_PART_remove_params, UF_PART_copy_exp_shallowly };  
        UF_CALL(UF_PART_ask_part_name(part, part_name));  
        UF_CALL(UF_PART_export_with_options(part_name, n_objects, objects, &options));  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API的二次开发代码，用于实现将指定数量的对象从一个工作部件复制到另一个工作部件的功能。
>
> 主要步骤如下：
>
> 1. 使用UF_ASSEM_ask_work_part()函数获取当前工作部件的tag。
> 2. 使用UF_PART_ask_part_name()函数获取当前工作部件的名称，并存入part_name数组。
> 3. 定义UF_PART_export_options_t结构体变量options，设置复制参数为FALSE，移除参数为TRUE，浅复制为TRUE。
> 4. 使用UF_PART_export_with_options()函数，以part_name为参数，将n_objects个对象从当前工作部件复制到另一个工作部件。
> 5. copy_objects()函数接收两个参数，n_objects指定要复制的对象数量，objects指向要复制的对象数组。
> 6. 函数首先获取当前工作部件，然后获取其名称，最后调用UF_PART_export_with_options()函数进行对象复制。
> 7. 通过设置options结构体，可以控制是否移除参数，是否进行浅复制等。
> 8. 整个函数实现了工作部件之间的对象复制功能，为NX二次开发提供了便利。
>
