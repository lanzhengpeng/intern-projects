### 【2434】report weight state 报告权重状态

#### 代码

```cpp
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

```

#### 代码解析

> 这段代码是一个用于报告权重状态的函数，其主要功能如下：
>
> 1. 接收两个参数：name表示权重名称，state表示权重状态。
> 2. 根据state的状态，构造不同的提示信息msg，并输出到NX的列表窗口。
> 3. 输出的提示信息包括权重名称和对应的权重状态描述。
> 4. 权重状态包括：无缓存、已缓存、明确指定、未知、继承、推导等。
> 5. 如果state不在上述状态范围内，则输出一个“未知”状态。
> 6. 使用UF_UI_write_listing_window函数将提示信息输出到NX的列表窗口。
> 7. 该函数可以用于报告各种权重状态的名称和描述，便于用户了解当前权重信息。
>
> 总的来说，这是一个用于输出权重状态信息到NX列表窗口的辅助函数。
>
