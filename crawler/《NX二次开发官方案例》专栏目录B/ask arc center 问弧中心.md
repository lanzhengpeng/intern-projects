### ask arc center 问弧中心

#### 代码

```cpp
    static void ask_arc_center(tag_t arc, double center[3])  
    {  
        UF_EVAL_p_t  
            evaluator;  
        UF_EVAL_arc_t  
            e_arc;  
        UF_CALL(UF_EVAL_initialize(arc, &evaluator));  
        UF_CALL(UF_EVAL_ask_arc(evaluator, &e_arc));  
        UF_VEC3_copy(e_arc.center, center);  
        UF_CALL(UF_EVAL_free(evaluator));  
    }

```

#### 代码解析

> 这段代码是用于提取NX CAD系统中弧的中心点的二次开发代码。
>
> 代码的功能包括：
>
> 1. 定义了一个名为ask_arc_center的静态函数，用于获取给定弧的中心点坐标。
> 2. 在函数内部，首先创建并初始化了一个弧的评估器对象。
> 3. 使用弧的评估器对象，查询并获取弧的相关信息，包括中心点坐标。
> 4. 将获取到的中心点坐标复制到输出参数center中。
> 5. 最后释放评估器对象，结束函数调用。
>
> 总的来说，这段代码实现了在NX CAD系统中提取弧的中心点坐标的功能，以便进行后续的二次开发操作。代码逻辑清晰，注释详细，易于理解和使用。
>
