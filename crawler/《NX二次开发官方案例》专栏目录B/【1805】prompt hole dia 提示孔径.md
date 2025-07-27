### 【1805】prompt hole dia 提示孔径

#### 代码

```cpp
    static logical prompt_hole_dia( char *prompt, char *dia )  
    {  
        char  
            menu[1][16] = { "diameter" };  
        double  
            values[1];  
        int  
            irc,  
            junk;  
        sscanf(dia, "%lf", &values[0]);  
        irc = uc1609(prompt, menu, 1, values, &junk);  
        if (irc == 3 || irc == 4)  
        {  
            sprintf(dia, "%lf", values[0]);  
            return TRUE;  
        }  
        else return FALSE;  
    }

```

#### 代码解析

> 这段NX二次开发代码定义了一个名为prompt_hole_dia的静态逻辑函数，用于获取和校验用户输入的直径值。
>
> 函数的主要功能包括：
>
> 1. 定义了一个提示信息数组menu和一个直径值数组values。
> 2. 使用sscanf函数从dia参数中读取直径值并存储在values数组中。
> 3. 调用uc1609函数显示一个输入对话框，提示用户输入直径值，并返回用户的选择。
> 4. 根据用户的选择，如果用户确认输入或取消输入，则将values数组中的值格式化回dia字符串，并返回TRUE。
> 5. 如果用户选择其他选项，则直接返回FALSE。
> 6. 函数通过返回值指示是否成功获取了有效的直径值。
>
> 总的来说，这段代码实现了通过对话框获取用户输入的直径值，并进行了简单的校验。
>
