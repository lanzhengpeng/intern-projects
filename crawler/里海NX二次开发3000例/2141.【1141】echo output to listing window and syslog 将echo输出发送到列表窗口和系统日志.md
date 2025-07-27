### 【1141】echo output to listing window and syslog 将echo输出发送到列表窗口和系统日志

#### 代码

```cpp
    #include <stdarg.h>  
    static void ECHO(const char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf(msg, sizeof(msg), format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }

```

#### 代码解析

> 这段代码是NX Open二次开发中的一个简单示例，主要实现了一个名为ECHO的函数，用于打印格式化的字符串到NX的日志窗口和系统日志中。
>
> 主要功能如下：
>
> 1. 定义了一个名为ECHO的静态函数，用于打印格式化的字符串。
> 2. 使用va_list实现可变参数，支持传入任意数量的参数。
> 3. 使用vsnprintf函数，将传入的参数按照指定格式格式化到一个固定长度的字符串数组中。
> 4. 打开NX的日志窗口，并将格式化后的字符串写入日志窗口。
> 5. 调用UF_print_syslog函数，将格式化后的字符串写入系统日志文件中。
> 6. 使用了NX Open提供的UI和日志相关的UF函数接口。
>
> 通过这个简单的示例，可以展示如何使用NX Open提供的函数接口，实现一个简单的日志打印功能。这对于在NX二次开发中进行调试和记录运行信息非常有用。
>
