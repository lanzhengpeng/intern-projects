### 【2058】report deformed definition data 报告变形定义数据

#### 代码

```cpp
    static void report_deformed_definition_data(  
        UF_ASSEM_deformed_definition_data_p_t deformed_def_data)  
    {  
        WRITE_D(deformed_def_data->part_occ);  
        WRITE_D(deformed_def_data->num_parents);  
        WRITEnT(deformed_def_data->num_parents, deformed_def_data->parents);  
        WRITEnS(deformed_def_data->num_parents, deformed_def_data->parent_prompts);  
        WRITE_D(deformed_def_data->num_expressions);  
        WRITEnT(deformed_def_data->num_expressions, deformed_def_data->expressions);  
        WRITEnS(deformed_def_data->num_expressions,   
            deformed_def_data->expression_prompts);  
        WRITE_S(deformed_def_data->help_string);  
    }

```

#### 代码解析

> 这段代码是一个用于报告NX CAD系统中变形定义数据的函数。
>
> 具体来说，该函数的名称为report_deformed_definition_data，其参数为一个指向NX变形定义数据结构的指针deformed_def_data。
>
> 函数内部使用了一系列宏来输出变形定义数据结构中的各个字段，包括：
>
> 这些宏通过调用不同的函数来输出不同类型的数据，例如WRITE_D输出整数，WRITE_S输出字符串，WRITEnT和WRITEnS输出整数数组和字符串数组。
>
> 总的来说，这个函数用于打印出一个NX变形定义数据结构中的所有字段，以方便调试和查看变形定义的内容。
>
