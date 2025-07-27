### 【0643】create circular iset 创建圆形特征集

#### 代码

```cpp
    static void create_circular_iset(int n_feats, tag_t *feats, double point[3],  
        double dir[3], int number, double angle, tag_t **isets)  
    {  
        int  
            err,  
            ii;  
        char  
            number_exp[10],  
            angle_exp[10];  
        tag_t  
            feat,  
            *items;  
        uf_list_p_t  
            feat_list;  
        sprintf(number_exp, "%d", number);  
        sprintf(angle_exp, "%f", angle);  
        feat_list = make_a_list(n_feats, feats);  
        UF_CALL(UF_MODL_create_circular_iset(0, point, dir, number_exp, angle_exp,  
            feat_list, &feat));  
        UF_CALL(UF_MODL_delete_list(&feat_list));  
        *isets = (items=(tag_t *)UF_allocate_memory(n_feats*sizeof(tag_t), &err));  
        for (ii = 0; ii < n_feats; ii++)  
            UF_CALL(UF_MODL_ask_instance_iset(feats[ii], &items[ii]));  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API实现的，用于创建一个圆形实例集。
>
> 代码的主要功能是：
>
> 1. 定义了create_circular_iset函数，用于创建一个圆形实例集。
> 2. 函数接受多个参数，包括特征数量n_feats、特征数组feats、点坐标point、方向dir、实例数量number、角度angle以及返回的实例集数组指针isets。
> 3. 使用sprintf函数将实例数量和角度转换为字符串格式。
> 4. 使用make_a_list函数创建特征列表。
> 5. 调用UF_MODL_create_circular_iset函数创建圆形实例集。
> 6. 使用UF_MODL_delete_list函数删除特征列表。
> 7. 使用UF_allocate_memory函数为实例集数组分配内存。
> 8. 遍历特征数组，调用UF_MODL_ask_instance_iset获取每个特征的实例集，并保存到实例集数组中。
> 9. 最后，通过实例集数组指针isets返回创建的实例集。
>
> 通过该函数，用户可以方便地创建圆形实例集，并获取每个特征的实例集信息。
>
