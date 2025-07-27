### 【1786】prompt for four integers 提示输入四个整数

#### 代码

```cpp
    static logical prompt_for_four_integers(char *prompt, char *item1, char *item2,  
        char *item3, char *item4, int *number1, int *number2, int *number3,   
            int *number4)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[4][16];  
        int  
            da[4];  
        strncpy(&menu[0][0], item1, 15);  
        strncpy(&menu[1][0], item2, 15);  
        strncpy(&menu[2][0], item3, 15);  
        strncpy(&menu[3][0], item4, 15);  
        menu[0][15] = '\0';  
        menu[1][15] = '\0';  
        menu[2][15] = '\0';  
        menu[3][15] = '\0';  
        da[0] = *number1;  
        da[1] = *number2;  
        da[2] = *number3;  
        da[3] = *number4;  
        resp = uc1607(prompt, menu, 4, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = da[0];  
            *number2 = da[1];  
            *number3 = da[2];  
            *number3 = da[3];  
            return TRUE;  
        }  
        else return FALSE;  
    }

```

#### 代码解析

> 这段代码是一个用于在NX中实现提示用户输入四个整数的函数。具体来说：
>
> 1. 函数名为prompt_for_four_integers，它接受一个提示字符串prompt，四个项目名称字符串item1、item2、item3、item4，以及四个整型变量的地址number1、number2、number3、number4。
> 2. 函数内部首先定义了一些局部变量，包括irc用于接收用户输入，resp用于接收对话框的返回值，menu用于存储项目名称，da用于存储输入的数值。
> 3. 接下来，函数通过strncpy将项目名称拷贝到menu数组中。
> 4. 然后，函数将传入的数值赋给da数组。
> 5. 函数调用NX的对话框函数uc1607，显示提示信息，并让用户输入四个数值。输入完成后，函数返回一个响应值resp。
> 6. 如果resp为3或4，表示用户输入了数值并关闭了对话框，此时将da中的数值赋值给传入的整型变量，并返回TRUE。否则返回FALSE。
> 7. 最终，函数通过返回值来表示用户是否成功输入了四个数值。
>
> 总的来说，这段代码实现了在NX中通过对话框提示用户输入四个整数的通用功能，适用于各种需要用户输入数值的场景。
>
