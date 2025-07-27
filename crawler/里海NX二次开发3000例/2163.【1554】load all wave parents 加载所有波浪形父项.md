### 【1554】load all wave parents 加载所有波浪形父项

#### 代码

```cpp
    static void load_all_wave_parents(tag_t part)  
    {  
        int  
            ii,  
            n_parts,  
            *statuses;  
        char  
            **file_names,  
            msg[133];  
        UF_CALL(UF_WAVE_load_parents(part, UF_ASSEM_all, &n_parts, &file_names,  
            &statuses));  
        if (n_parts > 0)  
        {  
            ECHO("Load notes:\n");  
            for (ii = 0; ii < n_parts; ii++)  
            {  
                UF_get_fail_message(statuses[ii], msg);  
                ECHO("  %s - %s\n", file_names[ii], msg);  
            }  
            ECHO("\n");  
            UF_free(statuses);  
            UF_free_string_array(n_parts, file_names);  
        }  
    }

```

#### 代码解析

> 这段代码是NX二次开发中用于加载所有波浪曲线父级文件的函数。
>
> 主要功能包括：
>
> 1. 调用UF_WAVE_load_parents函数，传入零件part和UF_ASSEM_all，以加载所有波浪曲线父级文件。
> 2. 获取加载的父级文件数量n_parts、文件名数组file_names和状态数组statuses。
> 3. 如果加载的父级文件数量大于0，则遍历文件名数组，使用UF_get_fail_message获取每个文件的状态信息，并打印文件名和状态信息。
> 4. 释放状态数组和文件名数组。
> 5. 通过ECHO函数打印加载信息。
>
> 总体来说，这段代码的作用是加载指定零件的所有波浪曲线父级文件，并打印加载结果，属于NX二次开发中用于管理波浪曲线的实用函数。
>
