### 【1176】ensure parent loaded 确保父级已加载

#### 代码

```cpp
    static void ensure_parent_loaded(tag_t object)  
    {  
        tag_t  
            instance,  
            part_occurrence;  
        UF_PART_load_status_t  
            load;  
        if (UF_ASSEM_is_occurrence(object))  
        {  
            part_occurrence = UF_ASSEM_ask_part_occurrence(object);  
            instance = UF_ASSEM_ask_inst_of_part_occ(part_occurrence);  
            if (instance == NULL_TAG)  
            {  
                ensure_parent_loaded(part_occurrence);  
                instance = UF_ASSEM_ask_inst_of_part_occ(part_occurrence);  
            }  
            UF_ASSEM_ensure_child_loaded(instance, &load);  
            if (load.n_parts > 0) report_load_status(&load);  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API的二次开发代码，其主要功能是确保NX装配体中的零件被加载。
>
> 具体来说，代码通过以下步骤实现：
>
> 1. 判断传入的tag是否为装配体，如果是，则获取该装配体中的第一个零件的实例。
> 2. 如果零件实例为空，递归调用ensure_parent_loaded函数，以确保父装配体被加载。
> 3. 调用UF_ASSEM_ensure_child_loaded函数，确保传入的零件实例被加载。
> 4. 如果零件实例被加载，则调用report_load_status函数报告加载状态。
> 5. report_load_status函数目前是空函数，需要开发者自行实现以报告加载状态。
> 6. 通过递归调用ensure_parent_loaded函数，可以确保整个装配体树中的所有零件都被加载。
>
> 总体而言，这段代码实现了递归加载NX装配体中的所有零件，以确保它们都可以被访问和操作。
>
