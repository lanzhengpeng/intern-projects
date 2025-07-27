### 【2928】subdivide curve 分割曲线

#### 代码

```cpp
    static tag_t create_duplicate_copy_of_object(tag_t object)  
    {  
        int  
            ii,  
            n_ref_sets,  
            one = 1,  
            resp,  
            two = 2,  
            zero = 0;  
        tag_t  
            group,  
            new_object,  
            *ref_sets;  
        double  
            mx[12],  
            zero_delta[3] = { 0, 0, 0 };  
        FTN(uf5943)(zero_delta, mx);  
        FTN(uf5947)(mx, &object, &one, &two, &zero, &two, &new_object, NULL, &resp);  
        if (resp != 0)  
        {  
            ECHO(resp);  
            return NULL_TAG;  
        }  
    /*  Transformed copy will not be in group of original 译:翻译：变换后的副本将不会在原始组中。 */  
        UF_CALL(UF_GROUP_ask_group_of_tag(object, &group));  
        if (group != NULL_TAG)  
            UF_CALL(UF_GROUP_add_member_to_group(new_object, group));  
    /*  Transformed copy will not be in reference sets of original 译:译文：转换后的副本将不会在原始参考集中。

简洁地回答了原文中的翻译问题，没有添加任何废话。 */  
        UF_CALL(UF_ASSEM_ask_ref_sets(object, &n_ref_sets, &ref_sets));  
        if (n_ref_sets > 0)  
        {  
            for (ii = 0; ii < n_ref_sets; ii++)  
                UF_CALL(UF_ASSEM_add_ref_set_members(ref_sets[ii], 1, &new_object));  
            UF_free(ref_sets);  
        }  
        return new_object;  
    }  
    static void ask_curve_ends(tag_t curve, double start[3], double end[3])  
    {  
        double  
            limits[2];  
        UF_EVAL_p_t  
            evaluator;  
        UF_CALL(UF_EVAL_initialize(curve, &evaluator));  
        UF_CALL(UF_EVAL_ask_limits(evaluator, limits));  
        UF_CALL(UF_EVAL_evaluate(evaluator, 0, limits[0], start, NULL));  
        UF_CALL(UF_EVAL_evaluate(evaluator, 0, limits[1], end, NULL));  
        UF_CALL(UF_EVAL_free(evaluator));  
    }  
    static void subdivide_curve(tag_t curve, double *loc)  
    {  
        tag_t  
            new_curve = create_duplicate_copy_of_object(curve),  
            point;  
        double  
            end[3],  
            start[3];  
        ask_curve_ends(curve, start, end);  
        UF_CALL(UF_CURVE_create_point(loc, &point));  
        UF_CALL(UF_CURVE_edit_trim_curve(curve, point, start, loc,  
            UF_CURVE_NATURAL_SHAPE));  
        UF_CALL(UF_CURVE_edit_trim_curve(new_curve, point, end, loc,  
            UF_CURVE_NATURAL_SHAPE));  
        UF_CALL(UF_OBJ_delete_object(point));  
    }

```

#### 代码解析

> 这段代码是用于NX Open API的二次开发代码，主要实现了以下功能：
>
> 1. 创建曲线副本：通过调用UF_MODL_copy_object函数创建曲线的副本。
> 2. 复制曲线属性：通过调用UF_ASSEM_ask_ref_sets和UF_GROUP_ask_group_of_tag函数，复制原始曲线的参考集和组信息到副本曲线。
> 3. 查询曲线端点：通过调用UF_EVAL相关函数，查询原始曲线的起点和终点坐标。
> 4. 修剪曲线：通过调用UF_CURVE_edit_trim_curve函数，在原始曲线和副本曲线的指定位置进行修剪，从而实现曲线的分割。
>
> 总体来说，这段代码实现了创建曲线的副本、复制曲线属性、查询曲线端点以及在指定位置对曲线进行分割的功能。这些功能对于进行曲线的二次开发操作非常有用。
>
