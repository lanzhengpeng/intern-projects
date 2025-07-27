### 【0186】ask drawing tag 询问图形标签

#### 代码

```cpp
    static tag_t ask_drawing_tag(char *dwg_name)  
    {  
        tag_t  
            dwg = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_OBJ_cycle_by_name_and_type(part, dwg_name, UF_drawing_type,  
            FALSE, &dwg));  
        return dwg;  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，主要功能是根据零件名称查询对应的图纸对象，具体步骤如下：
>
> 1. 首先定义一个tag_t类型的变量dwg，用于存储查询到的图纸对象句柄。
> 2. 然后调用UF_PART_ask_display_part()函数，获取当前打开的零件对象句柄，并将其赋值给变量part。
> 3. 接着调用UF_OBJ_cycle_by_name_and_type()函数，根据零件名称dwg_name和图纸类型UF_drawing_type，在当前零件下循环查询图纸对象。
> 4. 参数FALSE表示只查询顶层图纸对象。
> 5. 查询到的图纸对象句柄存储在dwg变量中。
> 6. 最后返回dwg变量，即查询到的图纸对象句柄。
>
> 总的来说，这段代码实现了根据零件名称查询对应顶层图纸对象的功能，返回值为查询到的图纸对象句柄，如果查询不到则返回NULL_TAG。
>
