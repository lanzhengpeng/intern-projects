### 【2022】report clone family treatment 报告克隆家族处理

#### 代码

```cpp
    static void report_clone_family_treatment(void)  
    {  
        UF_CLONE_family_treatment_t  
            family_treatment;  
        UF_CALL(UF_CLONE_ask_family_treatment(&family_treatment));  
        switch (family_treatment)  
        {  
            case UF_CLONE_treat_as_lost:  
                ECHO("family_treatment = UF_CLONE_treat_as_lost\n");  
                break;  
            case UF_CLONE_strip_family_status:  
                ECHO("family_treatment = UF_CLONE_strip_family_status\n");  
                break;  
            case UF_CLONE_give_error:  
                ECHO("family_treatment = UF_CLONE_give_error\n");  
                break;  
            default:  
                ECHO("family_treatment = %d (Unrecognized!)\n", family_treatment);  
                break;  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open C++二次开发代码，用于查询和报告NX中当前克隆体处理家族的策略。
>
> 主要功能包括：
>
> 1. 定义了UF_CLONE_family_treatment_t类型的变量family_treatment，用于存储查询到的克隆体处理家族的策略。
> 2. 使用UF_CLONE_ask_family_treatment函数查询当前克隆体处理家族的策略，并将结果存储在family_treatment变量中。
> 3. 通过switch语句判断查询到的策略，并输出相应的提示信息到控制台。
> 4. 如果查询到的策略不是已知的3种策略之一，则输出"Unrecognized!"的提示信息，并输出该策略的整数值。
>
> 总体来说，这段代码通过查询函数获取当前NX的克隆体处理家族的策略，并输出该策略的相关信息。这有助于了解当前NX的配置情况。
>
