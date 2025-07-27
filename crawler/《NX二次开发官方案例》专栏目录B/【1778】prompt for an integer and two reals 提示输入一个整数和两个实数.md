### 【1778】prompt for an integer and two reals 提示输入一个整数和两个实数

#### 代码

```cpp
    static logical prompt_for_an_integer_and_two_reals(char *prompt, char *item1,  
        char *item2, char *item3, int *number1, double *number2, double *number3)  
    {  
        int  
            ia[3],  
            ip[3] = { 101, 201, 201 },  
            ip6[3] = { 0, 1, 1 },  
            resp;  
        double  
            da[3];  
        char  
            ca[3][31],  
            menu[3][16];  
        strcpy(&menu[0][0], item1);  
        strcpy(&menu[1][0], item2);  
        strcpy(&menu[2][0], item3);  
        ia[0] = *number1;  
        da[1] = *number2;  
        da[2] = *number3;  
    /*  you could also use uc1608 like this:  
        resp = uc1608(prompt, menu, 3, ia, da, ip6);  
    */  
        resp = uc1613(prompt, menu, 3, ia, da, ca, ip);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = ia[0];  
            *number2 = da[1];  
            *number3 = da[2];  
            return TRUE;  
        }  
        else return FALSE;  
    }

```

#### 代码解析

> 这段代码是一个用于在NX中提示用户输入整型和两个实型数的函数。它使用了NX的交互式输入函数uc1613来实现交互式输入。
>
> 函数的主要功能包括：
>
> 1. 定义输入提示(prompt)、菜单项(item1,item2,item3)、输入类型(ip)、默认值(ip6)等。
> 2. 通过uc1613函数实现交互式输入，获取用户输入的整型和实型数。
> 3. 根据输入结果，如果用户确认输入，则将输入值赋值给参数number1,number2,number3，并返回TRUE。否则返回FALSE。
> 4. 通过参数传递和返回值，实现了获取用户输入的功能。
> 5. 使用NX的交互式输入函数，使得用户可以方便地在NX中输入参数，无需退出NX。
> 6. 参数的灵活设计使得该函数可以用于不同参数的输入。
>
> 综上所述，该函数通过NX的交互式输入功能，实现了参数的交互式输入，为NX二次开发提供了便捷的交互手段。
>
