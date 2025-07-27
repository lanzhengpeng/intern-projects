### 【2912】specify point 指定点

#### 代码

```cpp
    static logical specify_point(char *prompt, double loc[3])  
    {  
        int  
            ia2[2] = { 0,0 },  
            ip3 = 0,  
            irc;  
        irc = uc1616(prompt, ia2, ip3, loc);  
        if (irc == 5) return TRUE;  
        else return FALSE;  
    }

```

#### 代码解析

> 这是NX二次开发代码，用于提示用户指定一个点，并获取该点的坐标。
>
> 主要功能包括：
>
> 1. 定义了一个静态逻辑函数specify_point，用于提示用户指定点，并获取点的坐标。
> 2. 在函数内部，定义了整型数组ia2和整型变量ip3，用于存储用户输入的坐标。
> 3. 使用NX的API函数uc1616向用户显示提示信息prompt，并获取用户输入的坐标存入ia2和ip3中。
> 4. 判断返回码irc，如果为5表示用户成功指定了一个点，则返回true，否则返回false。
> 5. 外部调用specify_point函数，传入提示信息和坐标数组，即可提示用户指定点并获取坐标。
>
> 整体上，这段代码实现了在NX中提示用户指定点并获取坐标的功能。
>
