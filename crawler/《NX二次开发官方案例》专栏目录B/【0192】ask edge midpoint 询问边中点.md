### 【0192】ask edge midpoint 询问边中点

#### 代码

```cpp
    static void ask_edge_midpoint(tag_t edge, double point[3])  
    {  
        double  
            limits[2];  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_EVAL_p_t  
            eval;  
        UF_CALL(UF_EVAL_initialize(edge, &eval));  
        UF_CALL(UF_EVAL_ask_limits(eval, limits));  
        UF_CALL(UF_EVAL_evaluate(eval, 0, ((limits[0]+limits[1])/2), point, NULL));  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG,  
            UF_DISP_USE_ACTIVE_PLUS, point, &attrib, UF_DISP_POINT));  
        UF_CALL(UF_EVAL_free(eval));  
    }

```

#### 代码解析

> 这段代码是用于NX（Siemens NX）CAD软件的二次开发代码，其主要功能是计算并显示指定边的中心点。
>
> 主要步骤包括：
>
> 1. 初始化边评估对象，以获取边的参数信息。
> 2. 获取边的参数范围（即边的长度）。
> 3. 计算边的中心位置，即将参数范围的起始值和结束值相加后除以2。
> 4. 使用评估对象在计算出的中心位置参数值处求出边的中心点坐标。
> 5. 以临时点的形式在图形窗口中显示该中心点。
> 6. 释放评估对象，完成计算和显示。
>
> 该代码通过计算边的中心参数值，并在图形窗口中以点的形式显示，实现了在NX中获取并显示边的中心点的功能。通过二次开发，扩展了NX软件的功能，提高了工作效率。
>
