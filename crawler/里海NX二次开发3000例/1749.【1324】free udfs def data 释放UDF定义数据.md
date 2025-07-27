### 【1324】free udfs def data 释放UDF定义数据

#### 代码

```cpp
    static void free_udfs_def_data(UF_MODL_udfs_def_data_t udfs_data)  
    {  
        int  
            ii;  
        UF_free(udfs_data.frecs);  
        for (ii = 0; ii < udfs_data.num_exp; ii++)  
        {  
            UF_free(udfs_data.exp_defs[ii]->exp);  
            if (udfs_data.exp_defs[ii]->type == UF_UDFS_EXP_OPTION)  
                UF_free_string_array( udfs_data.exp_defs[ii]->num_def,  
                    udfs_data.exp_defs[ii]->define_value);  
        }  
        UF_free(udfs_data.exp_defs);  
        UF_free(udfs_data.refs);  
        UF_free_string_array(udfs_data.num_ref, udfs_data.ref_prompts);  
    }

```

#### 代码解析

> 这段NX Open UDFs二次开发代码实现了释放用户定义特征(UDFs)定义数据内存的功能。具体来说，代码步骤包括：
>
> 1. 释放函数参数中的UDFs数据结构中的frecs数组内存。
> 2. 遍历exp_defs数组，释放每个exp_defs结构中的exp字符串内存。
> 3. 如果exp_defs结构中的类型为UF_UDFS_EXP_OPTION，则释放define_value字符串数组内存。
> 4. 释放exp_defs数组和refs数组内存。
> 5. 释放ref_prompts字符串数组内存。
>
> 通过以上步骤，这段代码能够有效释放UDFs定义数据结构占用的内存资源，避免内存泄漏，保证UDFs二次开发模块的内存管理。
>
