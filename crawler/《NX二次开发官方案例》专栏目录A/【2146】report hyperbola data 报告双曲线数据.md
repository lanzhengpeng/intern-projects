### 【2146】report hyperbola data 报告双曲线数据

#### 代码

```cpp
    static void report_hyperbola_data(tag_t hyperbola)  
    {  
        logical  
            is_hyperbola;  
        UF_EVAL_p_t  
            evaluator;  
        UF_EVAL_hyperbola_t  
            eval_hyperbola;  
        UF_CURVE_conic_t  
            conic_data;  
        WRITE_D(hyperbola);  
        UF_CALL(UF_EVAL_initialize(hyperbola, &evaluator));  
        UF_CALL(UF_EVAL_is_hyperbola(evaluator, &is_hyperbola));  
        WRITE_L(is_hyperbola);  
        if (!is_hyperbola) return;  
        UF_CALL(UF_EVAL_ask_hyperbola(evaluator, &eval_hyperbola));  
        WRITE2F(eval_hyperbola.limits);  
        WRITE_F(eval_hyperbola.k1);  
        WRITE_F(eval_hyperbola.k2);  
        WRITE3F(eval_hyperbola.center);  
        WRITE3F(eval_hyperbola.x_axis);  
        WRITE3F(eval_hyperbola.y_axis);  
        UF_CALL(UF_EVAL_free(evaluator));  
        UF_CALL(UF_CURVE_ask_conic_data(hyperbola, &conic_data));  
        WRITE_D(conic_data.conic_type);  
        WRITE_F(conic_data.rotation_angle);  
        WRITE_F(conic_data.start_param);  
        WRITE_F(conic_data.end_param);  
        WRITE3F(conic_data.center);  
        WRITE_F(conic_data.k1);  
        WRITE_F(conic_data.k2);  
    }

```

#### 代码解析

> 这段代码是用于在NX Open API中获取和报告双曲线数据的函数。具体功能如下：
>
> 1. 定义了一个静态函数report_hyperbola_data，接受一个双曲线的tag作为参数。
> 2. 初始化评估器evaluator，用于后续查询双曲线的信息。
> 3. 使用UF_EVAL_is_hyperbola函数判断该曲线是否为双曲线，如果不是则返回。
> 4. 如果是双曲线，则使用UF_EVAL_ask_hyperbola函数获取双曲线的详细信息，包括极限位置、斜率、中心、坐标轴等。
> 5. 调用UF_CURVE_ask_conic_data函数获取双曲线的共面数据，如旋转角度、参数范围、中心、斜率等。
> 6. 通过WRITE系列函数打印双曲线的各种信息。
> 7. 最后释放评估器。
>
> 该函数的作用是遍历一个双曲线对象，获取并打印出其几何属性，用于调试或记录双曲线数据。
>
