### 【2203】report mc status 报告MC状态

#### 代码

```cpp
    static void report_mc_status(UF_ASSEM_mc_status_t stat)  
    {  
         switch(stat)  
         {  
           case UF_ASSEM_mc_solved:  
             printf("UF_ASSEM_mc_solve\n");  
             break;  
           case UF_ASSEM_mc_not_solved:  
             printf("UF_ASSEM_mc_NOT_solved\n");  
             break;  
           case UF_ASSEM_mc_no_mating_specified:  
             printf("UF_ASSEM_mc_no_mating_specified\n");  
             break;  
           default:  
             printf("Unknown status came back\n");  
         }  
    }

```

#### 代码解析

> 这段代码定义了一个名为report_mc_status的静态函数，用于报告NX装配中的MC状态。
>
> 主要功能包括：
>
> 该函数的主要作用是输出MC状态，帮助调试装配问题。
>
