### 【0612】create associative points spaced along curve 创建沿曲线等距的关联点

#### 代码

```cpp
    /*  
        When interactively editing the paramters of these points they may  
        flip to be offset off the curve in the other direction.  
        See PR 5861542.  
    */  
    static void create_associative_points_spaced_along_curve(tag_t curve,  
        double inc_percent)  
    {  
        tag_t  
            exp_tag,  
            from_point,  
            offset,  
            point_feat;  
        double  
            percent;  
        char  
            exp_str[30];  
        set_undo_mark("Create points on curve");  
        UF_CALL(UF_SO_create_scalar_double(curve, UF_SO_dont_update, 0.0, &offset));  
        UF_CALL(UF_SO_create_point_on_curve(curve, UF_SO_update_within_modeling,  
            curve, offset, &from_point));  
        for (percent = 0; percent <= 100; percent = percent+inc_percent)  
        {  
            sprintf(exp_str, "%f", percent);  
            strip_trailing_zeros(exp_str);  
            UF_CALL(UF_MODL_create_exp_tag(exp_str, &exp_tag));  
            UF_CALL(UF_SO_create_scalar_exp(curve, UF_SO_update_within_modeling,  
                exp_tag, &offset));  
        /*  When interactively editing the paramters of these points they may  
            flip to be offset off the curve in the other direction.  
            See PR 5861542  
        */  
            UF_CALL(UF_POINT_create_along_curve(curve, from_point, offset,  
                UF_SO_point_along_curve_percent, FALSE, &point_feat));  
        }  
    }

```

#### 代码解析

> 这段代码是用于在NX中创建沿曲线均匀分布的点的二次开发代码。
>
> 主要步骤包括：
>
> 1. 设置一个标记点，标记曲线的起始位置。
> 2. 遍历0%到100%的百分比，以指定的百分比增量(如10%)。
> 3. 在每个百分比位置，根据曲线参数方程计算该点的位置，并创建一个点特征。
> 4. 这个点特征会沿着曲线均匀分布。
>
> 需要注意的是，在交互式编辑这些点的参数时，它们可能会在曲线上下翻转，这可能是NX的bug。代码中提到了相关的bug报告号。
>
> 整体而言，这段代码实现了在NX中沿曲线创建均匀分布的点特征的功能。
>
