### 【0262】ask next curve 询问下一个曲线

#### 代码

```cpp
    static tag_t ask_next_curve(tag_t part, tag_t curve, int *type)  
    {  
        int  
            types[4] = { UF_line_type,  
                         UF_circle_type,  
                         UF_conic_type,  
                         UF_spline_type };  
        while ((*type < 4) &&  
            !UF_CALL(UF_OBJ_cycle_objs_in_part(part, types[*type], &curve)))  
        {  
            if (curve != NULL_TAG)   
                return curve;  
            else   
                *type = *type + 1;  
        }  
        *type = 0;  
        return NULL_TAG;  
    }

```

#### 代码解析

> 这段代码是用于NX(UG)的二次开发，目的是遍历NX部件中的所有曲线对象。
>
> 代码的主要功能是遍历部件中的曲线对象，并返回下一个曲线对象的tag。它接受两个参数：part表示部件的tag，curve表示当前曲线的tag。如果curve为NULL_TAG，表示从部件的第一个曲线开始遍历；否则，从curve表示的曲线开始遍历。
>
> 代码首先定义了一个数组types，其中包含了4种曲线类型：直线、圆、圆锥曲线和样条曲线。接着，代码使用一个while循环来遍历每种类型的曲线。循环条件是当前类型索引type小于4，且当前类型曲线在部件中不为空。在循环中，使用UF_OBJ_cycle_objs_in_part函数遍历当前类型的曲线，并返回下一个曲线的tag。如果下一个曲线不为空，则返回；否则，继续遍历下一类型的曲线。
>
> 如果所有类型的曲线都遍历完毕，则将type重置为0，并返回NULL_TAG表示没有更多曲线。
>
> 综上所述，这段代码实现了在NX部件中遍历所有类型曲线的功能，并且能够从指定的曲线开始遍历。
>
