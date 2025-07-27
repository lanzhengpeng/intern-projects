### 【1801】prompt for three strings 提示输入三个字符串

#### 代码

```cpp
    static logical prompt_for_three_strings(char *prompt, char *item1,  
        char *item2, char *item3, char *string1, char *string2, char *string3)  
    {  
        int  
            ia[3],  
            ip[3] = { 301, 301 },  
            resp;  
        double  
            da[3];  
        char  
            ca[3][31],  
            menu[3][16];  
        strcpy(&menu[0][0], item1);  
        strcpy(&menu[1][0], item2);  
        strcpy(&menu[2][0], item3);  
        strcpy(&ca[0][0], string1);  
        strcpy(&ca[1][0], string2);  
        strcpy(&ca[2][0], string3);  
        resp = uc1613(prompt, menu, 3, ia, da, ca, ip);  
        if (resp == 3 || resp == 4)  
        {  
            strcpy(string1, &ca[0][0]);  
            strcpy(string2, &ca[1][0]);  
            strcpy(string3, &ca[2][0]);  
            return TRUE;  
        }  
        else return FALSE;  
    }

```

#### 代码解析

> 根据代码的注释和逻辑，这是段NX Open C++ API开发的代码，主要功能是弹出一个提示框，提示用户输入三个字符串。作者设计这段代码的主要思路如下：
>
> 1. 定义了一个静态逻辑函数prompt_for_three_strings，用于提示用户输入三个字符串。
> 2. 函数接收提示文本、三个输入项名称、三个输入字符串作为参数。
> 3. 定义了数组，用于存储输入项的索引、指针、值和菜单数组。
> 4. 将输入项名称复制到菜单数组中。
> 5. 将输入字符串复制到字符数组中。
> 6. 调用uc1613函数，弹出一个选择输入的对话框。
> 7. 如果用户选择输入，将输入的字符串复制回传入的字符串参数。
> 8. 返回输入结果，如果用户选择了输入，则返回true，否则返回false。
>
> 这段代码的作用是在NX中实现一个弹窗提示用户输入三个字符串的功能，通过调用NX的对话框API实现交互式输入，获取用户输入的字符串。
>
