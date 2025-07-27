### 【0171】ask curve close end 询问曲线封闭端

#### 代码

```cpp
    static int ask_curve_close_end(tag_t curve, double pos[3])  
    {  
        double  
            dist[2],  
            parm;  
        UF_CALL(UF_MODL_ask_curve_parm(curve, pos, &parm, pos));  
        UF_CALL(UF_CURVE_ask_arc_length(curve, 0.0, parm, UF_MODL_UNITS_PART,  
            &dist[0]));  
        UF_CALL(UF_CURVE_ask_arc_length(curve, parm, 1.0, UF_MODL_UNITS_PART,  
            &dist[1]));  
        if (dist[0] < dist[1]) return 1;  
        else return 2;  
    }

```

#### 代码解析

> 这段代码是用于NX的二次开发，其主要功能是查询曲线是否闭合，并确定闭合曲线的起点和终点。
>
> 具体步骤如下：
>
> 1. 使用UF_MODL_ask_curve_parm函数，根据给定的位置点，查询曲线上的对应参数值parm。
> 2. 使用UF_CURVE_ask_arc_length函数，分别计算曲线从起点到参数值parm处的弧长dist[0]，以及从参数值parm到终点处的弧长dist[1]。
> 3. 根据两个弧长的大小关系，判断曲线是否闭合以及闭合曲线的起点和终点：如果dist[0] < dist[1]，则说明给定的位置点靠近曲线的起点，因此返回1，表示曲线闭合，起点在给定位置点的一侧。如果dist[0] >= dist[1]，则说明给定的位置点靠近曲线的终点，因此返回2，表示曲线闭合，终点在给定位置点的一侧。
> 4. 如果dist[0] < dist[1]，则说明给定的位置点靠近曲线的起点，因此返回1，表示曲线闭合，起点在给定位置点的一侧。
> 5. 如果dist[0] >= dist[1]，则说明给定的位置点靠近曲线的终点，因此返回2，表示曲线闭合，终点在给定位置点的一侧。
>
> 通过这种方法，可以根据给定的位置点，判断曲线是否闭合，并确定闭合曲线的起点和终点位置。
>
