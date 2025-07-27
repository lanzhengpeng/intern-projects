### 【2191】report logfile failure 报告日志文件失败

#### 代码

```cpp
    static void report_logfile_failure(UF_CLONE_log_file_failure_t *failure)  
    {  
        if (failure->line_number == -1) return;  
        ECHO("Error on Line %d:  input_part_name: %s invalid_token: %s\n",  
            failure->line_number, failure->input_part_name,  
            failure->invalid_token);  
        if (failure->input_part_name) UF_free(failure->input_part_name);  
        if (failure->invalid_token) UF_free(failure->invalid_token);  
    }

```

#### 代码解析

> 这段代码是NX二次开发中用于处理日志文件失败情况的函数。主要功能包括：
>
> 1. 输入参数：函数接收一个UF_CLONE_log_file_failure_t类型的结构体参数，该结构体包含了行号、输入零件名称和无效令牌等字段。
> 2. 行号判断：首先判断行号是否为-1，如果是，则直接返回，表示没有错误。
> 3. 错误输出：使用ECHO宏输出错误信息，包括行号、输入零件名称和无效令牌。
> 4. 释放内存：释放输入零件名称和无效令牌字符串占用的内存，避免内存泄漏。
> 5. 宏定义：ECHO是一个宏定义，用于输出信息。
> 6. 内存释放：使用UF_free函数释放内存。
> 7. 日志文件错误处理：此函数主要用于在读取NX日志文件时，处理解析错误的情况，能够输出错误信息并释放相关内存，避免内存泄漏。
> 8. 二次开发接口：该函数利用了NX的二次开发接口，通过结构体和宏定义实现了日志文件错误处理功能。
>
> 总体来说，这段代码实现了在读取NX日志文件时，对解析错误的处理，能够输出错误信息并释放相关内存，避免内存泄漏，体现了NX二次开发中对错误处理和内存管理的关注。
>
