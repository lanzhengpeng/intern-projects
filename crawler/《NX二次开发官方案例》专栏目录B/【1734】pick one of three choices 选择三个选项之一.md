### 【1734】pick one of three choices 选择三个选项之一

#### 代码

```cpp
    static logical pick_one_of_three_choices(char *prompt, char *option_one,  
        char *option_two, char *option_three, int *choice)  
    {  
        int  
            resp;  
        char  
            options[3][38];  
        strncpy(&options[0][0], option_one, 37);  
        strncpy(&options[1][0], option_two, 37);  
        strncpy(&options[2][0], option_three, 37);  
        options[0][37] = '\0';  
        options[1][37] = '\0';  
        options[2][37] = '\0';  
        resp = uc1603(prompt, *choice, options, 3);  
        if ((resp > 4) && (resp < 19))  
        {  
            *choice = resp - 4;  /* returns 1 , 2 or 3 译:根据上下文，returns 1, 2 or 3的翻译为：

返回1、2或3。 */  
            return TRUE;  
        }  
        else return FALSE;  
    }

```

#### 代码解析

> 这段代码是一个名为pick_one_of_three_choices的函数，用于在NX中实现一个简单的菜单选择功能。
>
> 函数的主要功能包括：
>
> 1. 提示用户输入一个选择，并提供三个选项。
> 2. 用户可以通过NX的对话框输入一个数字来选择一个选项。
> 3. 根据用户的选择，返回一个整型值，代表用户的选择（1、2或3）。
> 4. 如果用户输入了非法的数字，则函数返回FALSE，表示选择无效。
> 5. 如果用户输入了合法的数字，则函数返回TRUE，并将用户的选择作为整型参数返回。
> 6. 函数内部使用一个二维字符数组options来存储三个选项的文本，并调用uc1603函数显示提示信息和选项，获取用户输入。
> 7. 最后，根据用户输入的数字，计算出代表用户选择的整型值（1、2或3），并返回TRUE。
>
> 综上所述，这个函数实现了在NX中通过对话框让用户从三个选项中进行选择的功能，并返回用户的选择。
>
