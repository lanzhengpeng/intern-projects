### 【1986】report an integer 报告一个整数

#### 代码

```cpp
    #define SHOW_INTEGER(X)    (report_an_integer(#X, X))  
    static void report_an_integer(char *variable, int value)  
    {  
        char  
            msg[133];  
        sprintf(msg, "%s = %d\n", variable, value);  
        uc1601(msg, TRUE);  
    }

```

#### 代码解析

> 这段代码是用于在NX Open中的二次开发。具体来说：
>
> 1. #define SHOW_INTEGER(X) (report_an_integer(#X, X))定义了一个宏，用于打印一个整型变量的值和名称。其中，#X将变量名转换成字符串，report_an_integer是一个函数，用于报告整型变量的值。
> 2. static void report_an_integer(char *variable, int value)是一个静态函数，用于报告整型变量的值。它接受两个参数：变量的字符串名称和整型值。函数内部使用sprintf将变量名和值格式化成字符串，并调用uc1601函数显示在屏幕上。
> 3. uc1601函数应该是用于在NX中显示文本的函数。msg是格式化后的字符串，TRUE表示在屏幕上显示。
>
> 总的来说，这段代码通过宏和函数的封装，方便在NX二次开发中打印整型变量的值和名称，以提高开发效率。
>
