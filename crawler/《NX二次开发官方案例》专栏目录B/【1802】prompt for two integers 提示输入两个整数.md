### 【1802】prompt for two integers 提示输入两个整数

#### 代码

```cpp
    static logical prompt_for_two_integers(char *prompt, char *item1, char *item2,  
        int *number1, int *number2)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[2][16];  
        int  
            da[2];  
        strncpy(&menu[0][0], item1, 15);  
        strncpy(&menu[1][0], item2, 15);  
        menu[0][15] = '\0';  
        menu[1][15] = '\0';  
        da[0] = *number1;  
        da[1] = *number2;  
        resp = uc1607(prompt, menu, 2, da, &irc);  
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

> 这段代码是NX Open C++ API编写的二次开发代码，其主要功能是提示用户输入两个整数。
>
> 代码的主要逻辑如下：
>
> 1. 定义了函数prompt_for_two_integers，用于提示用户输入两个整数。
> 2. 函数接收提示信息(prompt)、两个项目名称(item1,item2)、两个整数值的地址(number1,number2)。
> 3. 定义了菜单数组menu，将项目名称存入其中。
> 4. 定义了整数值数组da，用于存储用户输入的整数值。
> 5. 调用uc1607函数显示菜单，提示用户输入整数值。uc1607函数是NX内置的对话框函数。
> 6. 根据用户输入的响应码resp判断，如果为3(确定)或4(应用)，则将输入的整数值存入number1和number2指向的变量。
> 7. 函数返回TRUE表示用户成功输入了两个整数值，返回FALSE表示用户取消了输入。
>
> 总体来看，这段代码实现了NX二次开发中提示用户输入两个整数值的功能，并可以获取用户输入的整数值。
>
