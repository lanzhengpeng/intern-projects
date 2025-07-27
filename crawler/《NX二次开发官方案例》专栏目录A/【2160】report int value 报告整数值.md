### 【2160】report int value 报告整数值

#### 代码

```cpp
    #define SHOW(X)    (report_int_value(#X, (X)))  
    static void report_int_value(char *variable, int value)  
    {  
        char  
            messg[133];  
        sprintf(messg, "%s = %d", variable, value);  
        printf("%s\n", messg);  
        uc1601(messg, TRUE);  
    }

```

#### 代码解析

> 这段代码是一段用于在NX二次开发中打印变量值的宏定义和函数实现。
>
> 1. 定义了宏SHOW(X)，它接受一个整数变量X作为参数，并调用report_int_value函数打印变量名和值。
> 2. report_int_value函数接受变量名和值作为参数，生成格式化的字符串，打印到控制台，并调用uc1601函数将字符串显示在NX的显示屏上。
> 3. SHOW宏简化了打印变量值的操作，只需一行代码即可实现变量值的打印和显示。
> 4. report_int_value函数将变量名和值格式化为字符串，并打印输出，以便调试和查看。
> 5. uc1601函数将字符串显示在NX的显示屏上，以直观查看变量值。
> 6. 整个实现简化了打印变量值的过程，方便调试和查看NX中的变量值。
>
> 总的来说，这段代码实现了在NX二次开发中打印变量值的功能，并提供了显示在NX显示屏上的支持。
>
