### 【2762】set feature layer 设置特征层

#### 代码

```cpp
    static void set_feature_layer(tag_t feat, int layer)  
    {  
        int  
            n_objs;  
        tag_t  
            geom,  
            *objs;  
        if (!UF_WAVE_ask_linked_feature_geom(feat, &geom) && geom)  
            set_layer_object_array(1, &geom, layer);  
        else  
        {  
            if (!UF_CURVE_ask_feature_curves(feat, &n_objs, &objs) && n_objs)  
            {  
                set_layer_object_array(n_objs, objs, layer);  
                UF_free(objs);  
            }  
            else  
            {  
                if (!UF_MODL_ask_feat_object(feat, &n_objs, &objs) && n_objs)  
                {  
                    set_layer_object_array(n_objs, objs, layer);  
                    UF_free(objs);  
                }  
            }  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API的一个二次开发示例，其功能是设置特征图层的图层。
>
> 主要步骤如下：
>
> 1. 获取特征链接的几何体，并设置其图层。
> 2. 如果特征没有链接几何体，则获取特征曲线，并设置其图层。
> 3. 如果特征既没有链接几何体，也没有曲线，则获取特征对象，并设置其图层。
>
> 具体实现包括：
>
> 该代码通过递归调用不同函数，实现了对特征图层设置的全面覆盖。
>
