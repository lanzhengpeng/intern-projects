### 【2433】report weight properties 报告重量属性

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
    static void report_weight_state(char *name, UF_WEIGHT_state_type_t state)  
    {  
        char  
            msg[133];  
        switch (state)  
        {  
            case UF_WEIGHT_no_cache:  
                sprintf(msg, "%s = UF_WEIGHT_no_cache (no data available)\n",  
                    name);  
                UF_UI_write_listing_window(msg);  
                break;  
            case UF_WEIGHT_cached:  
                sprintf(msg, "%s = UF_WEIGHT_cached (calculated)\n", name);  
                UF_UI_write_listing_window(msg);  
                break;  
            case UF_WEIGHT_asserted:  
                sprintf(msg, "%s = UF_WEIGHT_asserted (explicit)\n", name);  
                UF_UI_write_listing_window(msg);  
                break;  
            case UF_WEIGHT_unknown:  
                sprintf(msg, "%s = UF_WEIGHT_unknown (explicitly unknown)\n",  
                    name);  
                UF_UI_write_listing_window(msg);  
                break;  
            case UF_WEIGHT_inherited:  
                sprintf(msg, "%s = UF_WEIGHT_inherited (from assertion on part)\n",  
                    name);  
                UF_UI_write_listing_window(msg);  
                break;  
            case UF_WEIGHT_implied:  
                sprintf(msg, "%s = UF_WEIGHT_implied (derived)\n",  
                    name);  
                UF_UI_write_listing_window(msg);  
                break;  
            default:  
                sprintf(msg, "%s = %d? (unknown)\n", name, state);  
                UF_UI_write_listing_window(msg);  
        }  
    }  
    static void report_weight_properties( UF_WEIGHT_properties_t *properties)  
    {  
        UF_UI_open_listing_window();  
        report_weight_units("properties->units", properties->units);  
        report_weight_state("properties->cache_state", properties->cache_state);  
        if (properties->cache_state == UF_WEIGHT_no_cache) return;  
        WRITE_F(properties->accuracy);  
        WRITE_F(properties->density);  
        report_weight_state("properties->density_state", properties->density_state);  
        WRITE_F(properties->volume);  
        WRITE_F(properties->volume_error);  
        report_weight_state("properties->volume_state", properties->volume_state);  
        WRITE_F(properties->mass);  
        WRITE_F(properties->mass_error);  
        report_weight_state("properties->mass_state", properties->mass_state);  
        WRITE_F(properties->area);  
        WRITE_F(properties->area_error);  
        report_weight_state("properties->area_state", properties->area_state);  
        WRITE3F(properties->center_of_mass);  
        WRITE_F(properties->cofm_error);  
        report_weight_state("properties->cofm_state", properties->cofm_state);  
        WRITE3F(properties->moments_of_inertia);  
        WRITE3F(properties->products_of_inertia);  
        WRITE_F(properties->mofi_error);  
        report_weight_state("properties->mofi_state", properties->mofi_state);  
    }

```

#### 代码解析

> 这段代码是NX Open API中用于报告实体重量属性的函数。主要功能包括：
>
> 1. report_weight_units：报告重量属性的计量单位，包括千克米、磅英寸等。
> 2. report_weight_state：报告重量属性的缓存状态，包括无缓存、已缓存、明确指定、未知等。
> 3. report_weight_properties：报告实体的重量属性，包括单位、缓存状态、密度、体积、质量、面积、质心、惯性矩等，并调用前两个函数报告单位与状态。
> 4. WRITE_F：用于打印浮点数。
> 5. WRITE3F：用于打印3个浮点数组成的向量。
> 6. UF_UI_open_listing_window：打开NX的列表窗口，用于输出信息。
> 7. UF_UI_write_listing_window：向NX的列表窗口写入信息。
>
> 整个代码的目的是以可读的格式，输出实体的重量属性信息，以帮助用户理解实体的重量属性。
>
