### 【0224】ask finish symbol origin 询问完成符号的起点

#### 代码

```cpp
    static logical ask_finish_symbol_origin(tag_t symb, double origin[3])  
    {  
        int  
            ann_data[10],  
            ann_data_type,  
            ann_data_form,  
            cycle_flag = 0,  
            ii,  
            mask[4] = { 1,0,0,0 },  
            num_segments;  
        double  
            ann_origin[2],  
            endpoints[4],  
            radius_angle;  
        if (!UF_CALL(UF_DRF_ask_ann_data(&symb, mask, &cycle_flag, ann_data,  
            &ann_data_type, &ann_data_form, &num_segments, ann_origin,  
            &radius_angle)) && (cycle_flag != 0) && (num_segments > 0))  
        {  
            ii = 1;  
            UF_CALL(UF_DRF_ask_ann_line_seg_ends(&ii, ann_data, endpoints));  
            UF_VEC2_copy(&endpoints[0], origin);  
            origin[2] = 0.0;  
            return TRUE;  
        }  
        else  
            return FALSE;  
    }

```

#### 代码解析

> 这段代码用于获取NX中的注释符号的起点坐标，关键步骤如下：
>
> 1. 定义所需的变量，包括注释数据数组、数据类型、数据格式、循环标志、端点坐标等。
> 2. 调用UF_DRF_ask_ann_data函数，获取注释符号的数据。通过输入参数指定只获取起点数据，并设置循环标志。
> 3. 判断获取的数据是否有效，即循环标志不为0且段数大于0。
> 4. 如果数据有效，调用UF_DRF_ask_ann_line_seg_ends函数，获取注释线的第一个端点坐标，并赋值给origin数组。
> 5. 设置origin数组的Z坐标为0，因为我们只关心起点在XY平面上的坐标。
> 6. 如果获取到起点坐标，函数返回TRUE，否则返回FALSE。
>
> 通过以上步骤，这段代码实现了从NX注释符号中提取起点坐标的功能。
>
