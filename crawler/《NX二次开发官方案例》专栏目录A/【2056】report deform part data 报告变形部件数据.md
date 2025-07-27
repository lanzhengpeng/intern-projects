### 【2056】report deform part data 报告变形部件数据

#### 代码

```cpp
    static void report_deform_part_data(UF_ASSEM_deform_part_data_p_t deform_part)  
    {  
        WRITE_L(deform_part->convert_units_on_modified_parts);  
        WRITE_L(deform_part->convert_units_on_read_only_modified_parts);  
        WRITE_D(deform_part->part_occ_to_deform);  
        WRITE_D(deform_part->num_parents);  
        WRITEnTt(deform_part->num_parents, deform_part->old_parents);  
        WRITEnTt(deform_part->num_parents, deform_part->new_parents);  
        WRITE_D(deform_part->num_expressions);  
        WRITEnE(deform_part->num_expressions, deform_part->old_expressions);  
        WRITEnS(deform_part->num_expressions,  
            (char **)deform_part->new_expression_values);  
        WRITE_D(deform_part->deformed_feature_tag);  
    }

```

#### 代码解析

> 这段代码是用于报告NX模型中变形部件数据的函数。其基本功能包括：
>
> 1. 报告是否在修改过的只读部件上转换单位。
> 2. 报告是否在修改过的只读部件上转换单位。
> 3. 报告需要变形的部件实例。
> 4. 报告变形部件的父部件数量。
> 5. 报告变形部件的旧父部件列表。
> 6. 报告变形部件的新父部件列表。
> 7. 报告变形部件的表达式数量。
> 8. 报告变形部件的旧表达式列表。
> 9. 报告变形部件的新表达式值列表。
> 10. 报告变形部件的变形特征标签。
>
> 这个函数通过调用WRITE系列宏来输出上述信息，实现了对变形部件数据的详细报告，有助于调试和了解变形部件的状态。
>
