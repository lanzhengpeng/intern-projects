### 【0319】ask occurrence of promotion 查询提升出现的次数

#### 代码

```cpp
    static tag_t ask_occurrence_of_promotion(tag_t prom_object)  
    {  
        int  
            ii,  
            np;  
        tag_t  
            part_occ = NULL_TAG,  
           *path;  
        UF_CALL(UF_MODL_ask_promotion_path(prom_object, TRUE, &path, &np));  
        for (ii = np - 1; ii >= 0; ii--)  
            part_occ = UF_ASSEM_ask_part_occ_of_inst(part_occ, path[ii]);  
        UF_free(path);  
        return (UF_ASSEM_find_occurrence(part_occ, prom_object));  
    }

```

#### 代码解析

> 根据代码注释和NX Open API，这段代码的功能是查询给定提升对象(prom_object)在装配树中的出现次数。
>
> 主要步骤如下：
>
> 1. 调用UF_MODL_ask_promotion_path函数，获取从提升对象到顶层装配的路径path，并记录路径上的实例数np。
> 2. 从路径的最后一个实例开始，使用UF_ASSEM_ask_part_occ_of_inst函数，递归查询每个实例的父部件，直到路径的起始实例。
> 3. 调用UF_ASSEM_find_occurrence函数，在找到的父部件中查询提升对象的出现次数。
> 4. 释放路径数组path的内存。
> 5. 返回提升对象的出现次数。
>
> 这段代码主要用于NX装配模块，通过查询提升对象在装配树中的出现次数，可以检查模型中是否存在重复或冗余的组件，从而提高模型的准确性和完整性。
>
