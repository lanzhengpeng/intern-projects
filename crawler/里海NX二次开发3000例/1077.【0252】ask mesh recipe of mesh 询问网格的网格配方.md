### 【0252】ask mesh recipe of mesh 询问网格的网格配方

#### 代码

```cpp
    static tag_t ask_mesh_recipe_of_mesh(tag_t mesh)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *meshes,  
            part = UF_ASSEM_ask_work_part(),  
            recipe = NULL_TAG;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part( part, UF_sfem_mesh_recipe_type,  
            &recipe)) && (recipe != NULL_TAG))  
        {  
            UF_CALL(UF_SF_find_mesh(recipe, UF_SF_DIMENSION_ANY, &n, &meshes));  
            for (ii = 0; ii < n; ii++)  
                if (meshes[ii] == mesh)  
                {  
                    UF_free(meshes);  
                    return recipe;  
                }  
            if (n > 0) UF_free(meshes);  
        }  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是用于在NX中查找与给定网格对应的网格配方(recipe)的函数。
>
> 代码的主要功能是遍历当前工作部件中的所有网格配方，并检查每个配方中是否包含给定的网格。如果找到对应的网格，则返回该网格的配方；如果没有找到，则返回空标签(NULL_TAG)。
>
> 具体步骤如下：
>
> 1. 获取当前工作部件的标签。
> 2. 不断循环遍历部件中的网格配方，直到找到与给定网格匹配的配方或遍历完所有配方。
> 3. 对于每个配方，调用UF_SF_find_mesh函数获取配方中的所有网格，并检查是否包含给定网格。
> 4. 如果找到匹配的网格，则释放网格数组并返回配方标签。
> 5. 如果没有找到匹配的网格，则释放网格数组并继续遍历下一个配方。
> 6. 如果遍历完所有配方后仍未找到匹配的网格，则返回空标签(NULL_TAG)。
>
> 综上所述，这段代码的主要作用是在NX中查找与给定网格对应的网格配方。
>
