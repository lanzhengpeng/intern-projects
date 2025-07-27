### 【1806】prompt hole parms 提示孔参数

#### 代码

```cpp
    static logical prompt_hole_parms( char *angle, char *depth, char *dia)  
    {  
        char  
            menu[3][16] = { "diameter", "depth", "tip angle" };  
        double  
            values[3];  
        int  
            irc,  
            junk;  
        sscanf(dia, "%lf", &values[0]);  
        sscanf(depth, "%lf", &values[1]);  
        sscanf(angle, "%lf", &values[2]);  
        irc = uc1609("Enter simple hole parameters", menu, 3, values, &junk);  
        if (irc == 3 || irc == 4)  
        {  
            sprintf(dia, "%lf", values[0]);  
            sprintf(depth, "%lf", values[1]);  
            sprintf(angle, "%lf", values[2]);  
            return TRUE;  
        }  
        else return FALSE;  
    }

```

#### 代码解析

> 这段代码是一个用于在NX Open C++环境中获取用户输入参数的逻辑函数。
>
> 主要功能包括：
>
> 1. 定义一个菜单数组，包含3个选项：直径、深度和顶角。
> 2. 定义一个double类型的数组，用于存储用户输入的参数值。
> 3. 使用sscanf函数从输入的字符串中解析参数值到数组中。
> 4. 使用uc1609函数显示对话框，让用户输入参数。该函数可以返回用户的选择结果。
> 5. 如果用户点击了确定或应用，则将数组中的参数值格式化回字符串，并返回true。
> 6. 如果用户点击了取消，则返回false。
> 7. 函数的输入参数为3个字符串，分别用于存储直径、深度和顶角的值。
> 8. 函数的返回值为布尔类型，表示用户是否输入了参数。
>
> 总的来说，这个函数通过对话框获取用户输入的孔洞参数，并将参数值格式化回字符串，供后续代码使用。
>
