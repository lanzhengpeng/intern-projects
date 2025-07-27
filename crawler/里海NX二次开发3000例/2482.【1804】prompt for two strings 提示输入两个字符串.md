### 【1804】prompt for two strings 提示输入两个字符串

#### 代码

```cpp
    static logical prompt_for_two_strings(char *prompt, char *item1,  
        char *item2, char *string1, char *string2)  
    {  
        int  
            ia[2],  
            ip[2] = { 301, 301 },  
            resp;  
        double  
            da[2];  
        char  
            ca[2][31],  
            menu[2][16];  
        strcpy(&menu[0][0], item1);  
        strcpy(&menu[1][0], item2);  
        strcpy(&ca[0][0], string1);  
        strcpy(&ca[1][0], string2);  
        resp = uc1613(prompt, menu, 2, ia, da, ca, ip);  
        if (resp == 3 || resp == 4)  
        {  
            strcpy(string1, &ca[0][0]);  
            strcpy(string2, &ca[1][0]);  
            return TRUE;  
        }  
        else return FALSE;  
    }

```

#### 代码解析

> 这段代码是用于在NX Open中实现一个交互式提示框，用于提示用户输入两个字符串，并将输入的字符串存储在提供的变量中。其工作原理如下：
>
> 1. 定义了一个静态逻辑函数prompt_for_two_strings，它接受提示字符串、两个项目名称、两个字符串变量作为参数。
> 2. 在函数内部，定义了整型数组ia和ip，双精度数组da，字符数组ca和menu，分别用于存储用户的选择、输入的整数值、输入的字符串等。
> 3. 使用strcpy函数将项目名称和默认字符串复制到menu和ca数组中。
> 4. 调用NX Open提供的uc1613函数，显示交互式提示框，并等待用户输入。
> 5. 根据用户的选择(3或4)，将用户输入的字符串复制回传入的字符串变量中。
> 6. 最后返回用户是否成功输入了两个字符串的布尔值。
>
> 总体来说，这段代码实现了在NX Open中通过交互式提示框获取用户输入的功能，并将输入的字符串存储在指定的变量中。
>
