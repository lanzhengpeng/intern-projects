### 【1470】is extracted edge 提取的边

#### 代码

```cpp
    static logical is_extracted_edge(tag_t curve)  
    {  
        int  
            subtype,  
            type;  
        tag_t  
            group;  
        if (!UF_DRAW_ask_group_of_curve(curve, &group, &type, &subtype)  
            && (type == UF_curve_group_type)  
            && (subtype == UF_dropped_curve_group_subtype)) return TRUE;  
        else  
            return FALSE;  
    }

```

#### 代码解析

> 这段NX Open C++代码用于判断一个曲线是否是从装配体中提取的边缘。
>
> 代码逻辑如下：
>
> 1. 调用UF_DRAW_ask_group_of_curve函数，获取指定曲线的组信息，包括组标签、类型和子类型。
> 2. 检查函数调用是否成功，若失败则返回FALSE。
> 3. 检查曲线的组类型是否为UF_curve_group_type，表示这是一个曲线组。
> 4. 检查曲线的组子类型是否为UF_dropped_curve_group_subtype，表示这是从装配体中提取的曲线组。
> 5. 若上述两个条件都满足，则返回TRUE，表示这是一个从装配体中提取的边缘曲线。
> 6. 否则返回FALSE。
>
> 通过这个函数，可以判断一个曲线是否是从装配体中提取的边缘，从而进行后续处理，例如高亮显示或修改属性等。
>
