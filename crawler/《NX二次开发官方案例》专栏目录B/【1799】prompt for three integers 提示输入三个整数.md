### 【1799】prompt for three integers 提示输入三个整数

#### 代码

```cpp
    static logical prompt_for_three_integers(char *prompt, char *item1, char *item2,  
        char *item3, int *number1, int *number2, int *number3)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[3][16];  
        int  
            da[3];  
        strncpy(&menu[0][0], item1, 15);  
        strncpy(&menu[1][0], item2, 15);  
        strncpy(&menu[2][0], item3, 15);  
        menu[0][15] = '\0';  
        menu[1][15] = '\0';  
        menu[2][15] = '\0';  
        da[0] = *number1;  
        da[1] = *number2;  
        da[2] = *number3;  
        resp = uc1607(prompt, menu, 3, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = da[0];  
            *number2 = da[1];  
            *number3 = da[2];  
            return TRUE;  
        }  
        else return FALSE;  
    }

```

#### 代码解析

> 这段代码是一个名为prompt_for_three_integers的函数，用于在NX Open中获取用户输入的三个整数值。
>
> 主要功能包括：
>
> 1. 定义了一个静态函数prompt_for_three_integers，接收提示信息prompt，以及三个输入项名称item1、item2、item3，以及三个整数值的指针number1、number2、number3。
> 2. 在函数内部，定义了菜单数组menu和整数值数组da，用于存储输入项名称和初始数值。
> 3. 调用uc1607函数，向用户显示一个包含三个输入项的对话框，并允许用户修改数值。uc1607是一个未知的函数，可能是NX Open提供的用户交互函数。
> 4. 根据uc1607函数的返回值，判断用户是确认还是取消操作。如果确认，则将输入的数值赋值给number1、number2、number3，并返回TRUE；如果取消，则返回FALSE。
> 5. 通过指针参数的传递，函数可以修改外部变量的值。
> 6. 该函数封装了用户交互逻辑，可以简化获取用户输入三个整数的操作。
>
> 总的来说，这段代码通过静态函数封装了用户交互逻辑，实现了在NX Open中获取用户输入三个整数值的功能。
>
