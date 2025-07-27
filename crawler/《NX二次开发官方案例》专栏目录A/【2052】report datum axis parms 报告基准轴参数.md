### 【2052】report datum axis parms 报告基准轴参数

#### 代码

```cpp
    static void report_datum_axis_parms(tag_t daxis)  
    {  
        tag_t  
            feat;  
        double  
            origin[3],  
            normal[3];  
        UF_CALL(UF_MODL_ask_object_feat(daxis, &feat));  
        UF_CALL(UF_MODL_ask_datum_axis_parms(feat, origin, normal));  
        WRITE3F(origin);  
        WRITE3F(normal);  
    }

```

#### 代码解析

> 这段代码是用于查询NX中基准轴（datum axis）参数的函数，主要功能如下：
>
> 1. 获取基准轴对应的特征（feat）。
> 2. 查询基准轴的参数，包括原点坐标（origin）和法向量（normal）。
> 3. 打印原点坐标和法向量。
>
> 这个函数的目的是为了获取NX中已创建的基准轴的相关信息，包括原点和法向量，以便进行进一步的操作或分析。通过调用NX提供的UF_MODL_ask_object_feat和UF_MODL_ask_datum_axis_parms接口来实现这一功能。
>
