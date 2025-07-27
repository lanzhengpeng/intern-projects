### 【1763】print syslog 打印系统日志

#### 代码

```cpp
    static void print_syslog(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_print_syslog(msg, FALSE);  
    }

```

#### 代码解析

> 这段代码定义了一个名为print_syslog的函数，其功能是格式化打印系统日志消息。
>
> 主要步骤包括：
>
> 1. 定义一个字符数组msg，用于存储格式化后的消息字符串。
> 2. 使用va_list和va_start宏，获取可变参数列表，即format字符串后的参数。
> 3. 使用vsprintf函数，将可变参数列表按照format格式进行格式化，并存储到msg数组中。
> 4. 使用va_end宏结束可变参数列表的使用。
> 5. 调用UF_print_syslog函数，打印msg数组中的消息到系统日志。FALSE参数表示该消息不是从用户输入的。
> 6. 函数使用static修饰，表示其作用域为当前文件，只能在本文件中使用。
>
> 总体来说，这个函数封装了系统日志的打印，提供了更方便的日志输出方式。
>
