### 【2431】report weight exceptions 报告权重异常

#### 代码

```cpp
    static void report_weight_exception_reason(char *name,  
        UF_WEIGHT_exception_reason_t reason)  
    {  
        char  
            msg[133];  
        switch (reason)  
        {  
            case UF_WEIGHT_not_loaded:  
                sprintf(msg, "%s = UF_WEIGHT_not_loaded\n", name);  
                UF_UI_write_listing_window(msg);  
                break;  
            case UF_WEIGHT_insufficiently_loaded:  
                sprintf(msg, "%s = UF_WEIGHT_insufficiently_loaded\n", name);  
                UF_UI_write_listing_window(msg);  
                break;  
            case UF_WEIGHT_ref_set_absent:  
                sprintf(msg, "%s = UF_WEIGHT_ref_set_absent\n", name);  
                UF_UI_write_listing_window(msg);  
                break;  
            case UF_WEIGHT_comp_set_absent:  
                sprintf(msg, "%s = UF_WEIGHT_comp_set_absent\n", name);  
                UF_UI_write_listing_window(msg);  
                break;  
            case UF_WEIGHT_under_minimum_weight:  
                sprintf(msg, "%s = UF_WEIGHT_under_minimum_weight\n", name);  
                UF_UI_write_listing_window(msg);  
                break;  
            case UF_WEIGHT_over_maximum_weight:  
                sprintf(msg, "%s = UF_WEIGHT_over_maximum_weight\n", name);  
                UF_UI_write_listing_window(msg);  
                break;  
            case UF_WEIGHT_has_assertion:  
                sprintf(msg, "%s = UF_WEIGHT_has_assertion\n", name);  
                UF_UI_write_listing_window(msg);  
                break;  
            case UF_WEIGHT_insufficient_accuracy:  
                sprintf(msg, "%s = UF_WEIGHT_insufficient_accuracy\n", name);  
                UF_UI_write_listing_window(msg);  
                break;  
            default:  
                sprintf(msg, "%s = %d? (unknown)\n", name, reason);  
                UF_UI_write_listing_window(msg);  
        }  
    }  
    static void report_weight_exceptions(UF_WEIGHT_exceptions_t *exceptions)  
    {  
        int  
            ii;  
        UF_UI_open_listing_window();  
        WRITE_D(exceptions->n_exceptions);  
        for (ii = 0; ii < exceptions->n_exceptions; ii++)  
        {  
            WRITE_D(ii);  
            report_weight_exception_reason("exceptions->reasons[ii]",  
                exceptions->reasons[ii]);  
            WRITE_S(exceptions->part_names[ii]);  
            WRITE_S(exceptions->comp_names[ii]);  
            WRITE_L(exceptions->on_part[ii]);  
            WRITE_D(exceptions->repeat_counts[ii]);  
        }  
        UF_CALL(UF_WEIGHT_free_exceptions(exceptions));  
    }

```

#### 代码解析

> 这段代码是用于处理NX CAD系统中的重量分析中的异常情况的示例。具体功能如下：
>
> 1. report_weight_exception_reason函数用于报告单个重量异常的原因。它接受异常原因代码和异常名称，根据原因代码生成相应的错误消息，并写入NX的listing窗口。
> 2. report_weight_exceptions函数用于报告多个重量异常。它首先打开NX的listing窗口，然后遍历异常数组，对每个异常调用report_weight_exception_reason函数报告其详细信息，包括异常原因、零件名称、组件名称、是否在零件上以及重复次数。最后释放异常数组占用的内存。
> 3. 代码使用了NX提供的UF函数，如UF_UI_open_listing_window、UF_UI_write_listing_window、UF_WEIGHT_free_exceptions等，来与NX系统交互。
> 4. 代码中定义了重量异常的原因枚举，如UF_WEIGHT_not_loaded表示未加载，UF_WEIGHT_insufficiently_loaded表示加载不足等，以便生成准确的错误消息。
> 5. 通过报告重量异常，可以帮助用户了解重量分析失败的原因，从而采取相应的措施进行修正。
>
> 这段代码提供了处理NX重量分析异常情况的示例，用户可以根据需要进行修改和扩展，以处理更复杂的异常情况。
>
