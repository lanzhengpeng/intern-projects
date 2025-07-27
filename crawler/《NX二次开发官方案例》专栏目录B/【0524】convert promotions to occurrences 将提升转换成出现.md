### 【0524】convert promotions to occurrences 将提升转换成出现

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
    static void convert_promotions_to_occurrences(int n_objs, tag_t *objs)  
    {  
        int  
            ii;  
        for (ii = 0; ii < n_objs; ii++)  
            if (UF_OBJ_is_object_a_promotion(objs[ii]))  
                objs[ii] = ask_occurrence_of_promotion(objs[ii]);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，用于NX CAD系统的二次开发。其功能是从一组NX对象中找出推广对象，并将其转换为对应的实例存在对象。具体步骤如下：
>
> 1. 定义一个静态函数ask_occurrence_of_promotion，用于查询推广对象对应的实例存在对象。输入参数：推广对象标签prom_object调用UF_MODL_ask_promotion_path获取推广对象的路径遍历路径，通过UF_ASSEM_ask_part_occ_of_inst获取实例存在对象返回找到的实例存在对象标签
> 2. 输入参数：推广对象标签prom_object
> 3. 调用UF_MODL_ask_promotion_path获取推广对象的路径
> 4. 遍历路径，通过UF_ASSEM_ask_part_occ_of_inst获取实例存在对象
> 5. 返回找到的实例存在对象标签
> 6. 定义一个静态函数convert_promotions_to_occurrences，用于将一组对象中的推广对象转换为实例存在对象。输入参数：对象数量n_objs和对象标签数组objs遍历对象数组，使用UF_OBJ_is_object_a_promotion判断是否为推广对象如果是推广对象，调用ask_occurrence_of_promotion获取对应的实例存在对象标签，并替换数组中的推广对象标签
> 7. 输入参数：对象数量n_objs和对象标签数组objs
> 8. 遍历对象数组，使用UF_OBJ_is_object_a_promotion判断是否为推广对象
> 9. 如果是推广对象，调用ask_occurrence_of_promotion获取对应的实例存在对象标签，并替换数组中的推广对象标签
> 10. 通过以上两个函数，实现了从一组对象中找出推广对象，并将其转换为对应的实例存在对象的功能。
>
> 总体来说，这段代码利用NX Open C++ API提供的函数，实现了NX对象类型的转换，为NX系统的二次开发提供了便利。
>
