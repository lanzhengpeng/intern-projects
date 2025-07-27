### 【2222】report naming failures 报告命名失败

#### 代码

```cpp
    static void report_naming_failures(UF_CLONE_naming_failures_p_t status)  
    {  
        int  
            ii;  
        ECHO("%d Naming failures\n", status->n_failures);  
        if (status->n_failures > 0)  
        {  
            for (ii = 0; ii < status->n_failures; ii++)  
                ECHO(" %d.  %s -> %s - %s\n", ii+1, status->input_names[ii],  
                    status->output_names[ii], status->statuses[ii]);  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_failures, status->input_names);  
            UF_free_string_array(status->n_failures, status->output_names);  
        }  
    }

```

#### 代码解析

> 这段代码是用于处理NX二次开发中的命名失败报告的函数。
>
> 主要功能包括：
>
> 1. 打印命名失败的个数。
> 2. 遍历并打印每个失败的输入名称、输出名称和状态信息。
> 3. 释放相关的动态内存空间。
>
> 这个函数通过参数接收一个结构体，其中包含了失败的个数、输入名称数组、输出名称数组和状态数组。函数首先打印出失败的总数，然后逐个打印每个失败的详细信息，最后释放掉动态分配的内存空间，以避免内存泄露。
>
> 这个函数的作用是在NX二次开发过程中，当发生命名失败时，可以调用该函数来报告失败的详细信息，以便调试和定位问题。
>
