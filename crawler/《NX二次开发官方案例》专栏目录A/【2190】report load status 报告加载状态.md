### 【2190】report load status 报告加载状态

#### 代码

```cpp
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        int  
            ii;  
        char  
            msg[133];  
        if (status->n_parts > 0)  
        {  
            ECHO("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_get_fail_message(status->statuses[ii], msg);  
                ECHO("  %s - %s\n", status->file_names[ii], msg);  
            }  
            ECHO("\n");  
            UF_CALL(UF_PART_free_load_status(status));  
        }  
    }

```

#### 代码解析

> 这段代码是用于报告NX零件加载状态的二次开发函数。
>
> 主要功能包括：
>
> 1. 获取加载状态，通过UF_PART_load_status_p_t结构体传入。
> 2. 检查零件数量，如果数量大于0，则进行下一步处理。
> 3. 遍历每个零件的加载状态，并使用UF_get_fail_message函数获取加载失败的信息。
> 4. 将文件名和加载失败信息打印输出。
> 5. 使用UF_PART_free_load_status释放加载状态结构体。
> 6. 通过ECHO宏打印输出信息到日志文件。
> 7. 通过UF_CALL宏调用UF函数，并处理函数调用失败的情况。
>
> 总的来说，这段代码的功能是获取NX零件加载状态，并打印输出加载失败的零件信息，以供调试和日志记录。
>
