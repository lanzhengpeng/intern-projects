### 【0354】ask surface area of sheet 询问表面积

#### 代码

```cpp
    static double ask_surface_area_of_sheet(tag_t sheet)  
    {  
        int  
            units;  
        tag_t  
            part = UF_PART_ask_display_part();  
        double  
            parameter[11] = { 0.99, 1,2,3,4,5,6,7,8,9,10 },  
            results[47],  
            statistics[13];  
        UF_CALL(UF_PART_ask_units(part, &units));  
        units = 5-(2*units); /*  UF_PART_METRIC  -> 3 = Grams and centimeters  
                                 UF_PART_ENGLISH -> 1 = Pounds and inches */  
        UF_CALL(UF_MODL_ask_mass_props_3d(&sheet, 1, 2, units, 1.0, 1, parameter,  
            results, statistics));  
        return results[0];  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，其主要功能是查询NX模型中一个图元的表面积。
>
> 代码首先获取当前显示的部件，并获取该部件的长度单位（公制或英制）。接着，调用UF_MODL_ask_mass_props_3d函数计算图元的质心、惯性矩等属性，其中参数units设置为根据长度单位计算质量的单位（公制为克/厘米3，英制为磅/英寸3）。计算完成后，返回results数组中的第一个元素，即图元的表面积。
>
> 该函数通过NX Open API实现了一个简单的查询功能，可根据不同的长度单位返回图元的表面积。
>
