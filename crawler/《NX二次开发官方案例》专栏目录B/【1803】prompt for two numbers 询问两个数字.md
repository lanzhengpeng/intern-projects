### 【1803】prompt for two numbers 询问两个数字

#### 代码

```cpp
    static logical prompt_for_two_numbers(char *prompt, char *item1, char *item2,  
        double *number1, double *number2)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[2][16];  
        double  
            da[2];  
        strncpy(&menu[0][0], item1, 15);  
        strncpy(&menu[1][0], item2, 15);  
        menu[0][15] = '\0';  
        menu[1][15] = '\0';  
        da[0] = *number1;  
        da[1] = *number2;  
        resp = uc1609(prompt, menu, 2, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = da[0];  
            *number2 = da[1];  
            return TRUE;  
        }  
        else return FALSE;  
    }

```

#### 代码解析

> 这段代码是用于在NX中创建一个对话框，提示用户输入两个数字，并将输入的数字存储在指定的变量中。主要功能包括：
>
> 1. 定义了一个静态函数prompt_for_two_numbers，用于创建对话框并处理用户输入。
> 2. 函数参数包括提示信息、两个输入项的名称、两个数字变量的地址。
> 3. 函数内部使用一个二维字符数组menu存储输入项的名称，使用一个double数组da存储两个数字变量的值。
> 4. 调用uc1609函数显示对话框，让用户输入或选择两个数字。该函数会阻塞直到用户操作结束。
> 5. 根据uc1609的返回值判断用户是否输入了两个数字，如果是，则更新传入的数字变量。
> 6. 函数返回逻辑值，表示用户是否输入了两个数字。
> 7. 该函数可以被其他代码调用，用于提示用户输入两个数字，并将输入结果存储在指定的变量中。
>
> 总体来说，这段代码实现了在NX中通过对话框提示用户输入两个数字，并将输入结果存储在外部变量的功能。
>
