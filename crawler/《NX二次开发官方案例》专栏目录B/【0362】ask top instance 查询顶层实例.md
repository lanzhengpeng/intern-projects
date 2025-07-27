### 【0362】ask top instance 查询顶层实例

#### 代码

```cpp
    static tag_t ask_top_instance(tag_t occurrence)  
    {  
        tag_t  
            instance,  
            last,  
            part_occ;  
        do  
        {  
            last = occurrence;  
            occurrence = UF_ASSEM_ask_part_occurrence(occurrence);  
            instance = UF_ASSEM_ask_inst_of_part_occ(occurrence);  
        } while (instance != NULL_TAG);  
        return UF_ASSEM_ask_inst_of_part_occ(last);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，用于查询NX装配体中指定零件的顶层实例。
>
> 主要步骤如下：
>
> 1. 定义函数ask_top_instance，接收一个参数occurrence，表示要查询的零件在装配体中的实例。
> 2. 在函数内部，定义三个tag_t类型的变量instance、last和part_occ。
> 3. 使用do-while循环，循环调用UF_ASSEM_ask_part_occurrence函数，获取零件的上层实例，并更新occurrence。
> 4. 在每次循环中，调用UF_ASSEM_ask_inst_of_part_occ函数，获取当前实例对应的零件实例。
> 5. 如果零件实例为空，说明已到达顶层实例，跳出循环。
> 6. 最后返回last实例对应的零件实例，即顶层实例。
>
> 通过递归查询零件的上层实例，最终找到顶层实例，实现了查询零件在装配体中的顶层实例的功能。
>
