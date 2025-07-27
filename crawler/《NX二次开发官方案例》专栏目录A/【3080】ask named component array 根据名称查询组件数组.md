### 【3080】ask named component array 根据名称查询组件数组

#### 代码

```cpp
    static tag_t ask_named_component_array(char *name)  
    {  
        int  
            ii,  
            n,  
            type;  
        tag_t  
            *arrays,  
            *comps,  
            found = NULL_TAG;  
        UF_ASSEM_iset_array_data_t  
            iset_data;  
        UF_ASSEM_mc_array_data_t  
            mc_data;  
        UF_ASSEM_ask_arrays_in_part(UF_PART_ask_display_part(), &n, &arrays);  
        for (ii = 0; ii < n; ii++)  
        {  
            UF_CALL(UF_ASSEM_ask_type_of_array(arrays[ii], &type));  
            if (type == UF_ASSEM_ISET_ARRAY)  
            {  
                UF_CALL(UF_ASSEM_ask_iset_array_data(arrays[ii],&iset_data,&comps));  
                if (!strcmp(iset_data.array_name, name)) break;  
            }  
            else  
            {  
                UF_CALL(UF_ASSEM_ask_mc_array_data(arrays[ii], &mc_data, &comps));  
                if (!strcmp(mc_data.array_name, name)) break;  
            }  
            UF_free(comps);  
        }  
        if (ii < n) found = arrays[ii];  
        UF_free(arrays);  
        return found;  
    }

```

#### 代码解析

> 这段NX Open C++代码的目的是查询NX部件中名为“name”的阵列组件。
>
> 主要步骤包括：
>
> 1. 获取当前显示部件中的所有阵列组件。
> 2. 遍历每个阵列组件，判断其类型是集合阵列还是模型阵列。
> 3. 如果是集合阵列，则获取其数据结构，并判断其名称是否等于“name”。
> 4. 如果是模型阵列，则获取其数据结构，并判断其名称是否等于“name”。
> 5. 如果找到名称匹配的阵列组件，则返回该组件的tag。
> 6. 如果遍历完所有阵列组件后没有找到匹配的，则返回NULL_TAG。
> 7. 最后释放申请的内存。
>
> 该函数实现了根据名称查询NX部件中的阵列组件的功能。
>
