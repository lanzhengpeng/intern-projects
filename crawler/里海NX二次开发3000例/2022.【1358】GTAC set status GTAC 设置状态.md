### 【1358】GTAC set status GTAC 设置状态

#### 代码

```cpp
    static void GTAC_set_status(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_CALL(UF_UI_set_status(msg));  
    }

```

#### 代码解析

> 这段代码是NX Open的二次开发代码，用于设置NX用户界面状态栏的提示信息。代码的主要功能是接受可变参数，将其格式化为字符串，并调用UF_UI_set_status函数将字符串显示在NX的状态栏上。
>
> 具体来说：
>
> 1. 定义了一个名为GTAC_set_status的静态函数，接受可变参数。
> 2. 定义了一个字符数组msg，用于存储格式化后的提示信息。
> 3. 使用va_start和va_end宏来处理可变参数，使用vsprintf函数将参数格式化为字符串，并存储在msg数组中。
> 4. 调用UF_UI_set_status函数，将msg数组中的字符串显示在NX的状态栏上。
> 5. UF_UI_set_status函数是NX提供的用户界面函数，用于设置状态栏提示信息。
> 6. UF_CALL宏用于调用NX的UF函数，并处理可能发生的错误。
>
> 总的来说，这段代码提供了一个简单的接口，用于在NX的二次开发中设置状态栏的提示信息，使用户能够看到相关的提示信息。
>
