### 【1774】prompt block parms 提示块参数

#### 代码

```cpp
    static logical prompt_block_parms(char *sides[3])  
    {  
        char  
            menu[3][16] = { "height", "width", "length" };  
        double  
            values[3];  
        int  
            junk,  
            i,  
            irc;  
        for (i=0; i<3; i++) sscanf(sides[i], "%lf", values[i]);  
        irc = uc1609("Enter block parameters", menu, 3, values, &junk);  
        if (irc == 3 || irc == 4)  
        {  
            for (i=0; i<3; i++) sprintf(sides[i], "%f", values[i]);  
            return TRUE;  
        }  
        else return FALSE;  
    }

```

#### 代码解析

> 这段代码定义了一个名为prompt_block_parms的静态逻辑函数，用于提示用户输入块体的参数。
>
> 主要功能包括：
>
> 1. 定义了一个包含3个字符串的二维数组menu，分别表示块体的height、width和length。
> 2. 定义了一个包含3个double类型元素的数组values，用于存储用户输入的块体参数值。
> 3. 使用for循环，通过sscanf函数从字符串参数sides中解析出3个double类型的值，并存储到values数组中。
> 4. 调用uc1609函数，提示用户输入块体参数，并显示menu数组中的3个选项。
> 5. 根据uc1609函数的返回值，如果为3或4，表示用户成功输入了参数，则将values数组中的值格式化为字符串，并存储到sides数组中。
> 6. 如果uc1609的返回值不是3或4，表示用户取消了输入，则返回FALSE。
> 7. 如果用户成功输入了参数，则返回TRUE。
>
> 综上所述，该函数的作用是提示用户输入块体的height、width和length参数，并返回用户是否成功输入了这些参数。
>
