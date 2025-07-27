### 【2435】report weight units 报告权重单位

#### 代码

```cpp
    static void report_weight_units(char *name, UF_WEIGHT_units_type_t units)  
    {  
        char  
            msg[133];  
        switch (units)  
        {  
            case UF_WEIGHT_units_km:  
                sprintf(msg, "%s = UF_WEIGHT_units_km (Kilograms and meters)\n",  
                    name);  
                UF_UI_write_listing_window(msg);  
                break;  
            case UF_WEIGHT_units_li:  
                sprintf(msg, "%s = UF_WEIGHT_units_li (Pounds and inches)\n", name);  
                UF_UI_write_listing_window(msg);  
                break;  
            case UF_WEIGHT_units_lf:  
                sprintf(msg, "%s = UF_WEIGHT_units_lf (Pounds and feet)\n", name);  
                UF_UI_write_listing_window(msg);  
                break;  
            case UF_WEIGHT_units_gm:  
                sprintf(msg, "%s = UF_WEIGHT_units_gm (Grams and millimeters)\n",  
                    name);  
                UF_UI_write_listing_window(msg);  
                break;  
            case UF_WEIGHT_units_gc:  
                sprintf(msg, "%s = UF_WEIGHT_units_gc (Grams and centimeters)\n",  
                    name);  
                UF_UI_write_listing_window(msg);  
                break;  
            default:  
                sprintf(msg, "%s = %d? (unknown)\n", name, units);  
                UF_UI_write_listing_window(msg);  
        }  
    }

```

#### 代码解析

> 这是一段NX Open C++二次开发代码，用于报告权重单位。其主要功能包括：
>
> 1. 定义了一个名为report_weight_units的静态函数，用于报告权重单位。
> 2. 函数接受两个参数：name表示单位名称，units表示单位类型。
> 3. 根据units的值，使用switch语句判断单位类型，并格式化输出不同的提示信息。
> 4. 使用sprintf函数将提示信息格式化输出到字符串msg中。
> 5. 调用UF_UI_write_listing_window函数将提示信息输出到NX的列表窗口。
> 6. 如果units的值不是已知单位，则输出"unknown"。
> 7. 通过调用这个函数，可以方便地报告权重单位类型，用于调试和查看程序中使用的权重单位。
>
> 综上所述，这段代码实现了在NX中报告权重单位的功能，使用静态函数封装，调用方便，代码结构清晰。
>
