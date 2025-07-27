### 【0677】create dumb curve wireframe container 创建哑曲线线框容器

#### 代码

```cpp
    static tag_t create_dumb_curve_wireframe_container(int n_curves, tag_t *curves)  
    {  
        int  
            rule_types[1] = { UF_SC_CURVE_DUMB_CURVES };  
        tag_t  
            collector;  
        UF_SC_input_data_t  
            rule;  
        UF_MODL_init_sc_input_data(UF_SC_CURVE_DUMB_CURVES, &rule);  
        rule.curve_dumb_input.num_curves = n_curves;  
        rule.curve_dumb_input.curves = curves;  
        UF_CALL(UF_MODL_create_smart_wireframe_container(curves[0], 1, rule_types,  
            &rule, &collector));  
        return collector;  
    }

```

#### 代码解析

> 根据代码的注释和逻辑，可以总结如下：
>
> 1. 该代码用于在NX中创建一个包含多个曲线的智能线框容器。
> 2. 函数名为create_dumb_curve_wireframe_container，接受参数n_curves表示曲线数量，curves为曲线数组。
> 3. 定义了一个规则类型数组，只包含一种规则类型UF_SC_CURVE_DUMB_CURVES，表示曲线为简单曲线。
> 4. 初始化智能线框输入数据，并设置曲线数量和曲线数组。
> 5. 调用UF_MODL_create_smart_wireframe_container函数，传入曲线、规则类型、规则和输出容器，创建智能线框容器。
> 6. 返回创建的容器。
>
> 综上所述，该代码实现了在NX中创建一个包含多条简单曲线的智能线框容器，并返回容器句柄。
>
