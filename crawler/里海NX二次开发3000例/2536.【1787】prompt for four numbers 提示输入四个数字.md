### 【1787】prompt for four numbers 提示输入四个数字

#### 代码

```cpp
    static logical prompt_for_four_numbers(char *prompt,  
        char *item1, char *item2, char *item3, char *item4,  
        double *number1, double *number2, double *number3, double *number4)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[4][16];  
        double  
            da[4];  
        strncpy(&menu[0][0], item1, 15);  
        strncpy(&menu[1][0], item2, 15);  
        strncpy(&menu[2][0], item3, 15);  
        strncpy(&menu[3][0], item4, 15);  
        da[0] = *number1;  
        da[1] = *number2;  
        da[2] = *number3;  
        da[3] = *number4;  
        resp = uc1609(prompt, menu, 4, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = da[0];  
            *number2 = da[1];  
            *number3 = da[2];  
            *number4 = da[3];  
            return TRUE;  
        }  
        else return FALSE;  
    }

```

#### 代码解析

> 这段代码定义了一个名为prompt_for_four_numbers的静态逻辑函数，用于提示用户输入四个数字，并更新相应的变量。
>
> 主要功能包括：
>
> 1. 定义了函数的参数，包括提示信息、四个输入项的描述、以及四个用于存储输入数字的指针变量。
> 2. 定义了局部变量，包括菜单数组、数字数组、以及返回码。
> 3. 将输入项描述拷贝到菜单数组中，并将输入数字拷贝到数字数组中。
> 4. 调用uc1609函数，显示提示信息和菜单，获取用户输入，并更新数字数组。
> 5. 判断用户输入，如果为3或4，表示用户输入了数字，则将数字数组更新回输入数字指针，并返回true。
> 6. 如果用户没有输入数字，则返回false。
> 7. 函数的返回值为逻辑类型，用于指示用户是否成功输入了数字。
>
> 总的来说，这个函数通过弹窗提示用户输入四个数字，并更新外部变量。它封装了获取用户输入的细节，对外提供简洁的接口。
>
