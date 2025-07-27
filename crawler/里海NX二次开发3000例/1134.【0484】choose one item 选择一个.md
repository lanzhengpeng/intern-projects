### 【0484】choose one item 选择一个

#### 代码

```cpp
    static int choose_one_item(char *prompt, char **choices, int n_choices)  
    {  
        int  
            a = 0,  
            ii,  
            resp,  
            z;  
        char  
            options[14][38];  
        do  
        {  
            z = (n_choices - a < 14) ? n_choices - a : 14;  
            for (ii = 0; ii < z; ii++)  
            {  
                strncpy(&options[ii][0], choices[a+ii], 37);  
                options[ii][37] = '\0';  
            }  
            if ((z == 14) && ((a + z) < n_choices))  
                strcpy(&options[13][0], "More...");  
            switch (resp = uc1603(prompt, 0, options, z))  
            {  
                case 1:                     /* Back 译:Back */  
                    if (a == 0) return -1;  
                    a = a - 13;  
                    break;  
                case 2:                     /* Cancel 译:Cancel */  
                    return -1;  
                    break;  
                case 18:                    /* More... 译:More...的翻译为：更多... */  
                    if ((a + z) < n_choices)  
                    {  
                        a = a + 13;  
                        break;  
                    }  
                default:                    /* Picked one 译:选择一个 */  
                    return a + resp - 5;  
                    break;  
            }  
        } while (TRUE);  
    }

```

#### 代码解析

> 这段代码是一个C语言函数，用于在菜单中选择一个选项。以下是该函数的主要特点：
>
> 1. 函数名和参数：函数名为choose_one_item，接受三个参数：提示信息prompt，选项数组choices，以及选项数量n_choices。
> 2. 循环选择：函数使用一个无限循环，在每次循环中显示14个选项，并允许用户选择一个。如果选项总数超过14个，则会显示"更多…"选项以继续显示其他选项。
> 3. 返回值：函数的返回值是用户选择的选项在choices数组中的索引。如果用户取消或返回，则返回-1。
> 4. 选项处理：如果用户选择"更多…“，则继续显示下一组14个选项。如果用户选择"返回”，则回到上一组选项。其他情况下，返回用户选择的选项索引。
> 5. 字符串处理：使用strncpy和strcpy来处理选项字符串，确保不会越界。
> 6. 使用宏定义：使用宏定义来处理不同选项的返回值，如返回值1对应"返回"，2对应"取消"，18对应"更多…"。
> 7. 用户输入：用户输入通过uc1603函数获取，该函数的具体实现没有给出，但应该是用来显示提示和选项，并获取用户选择的返回值。
>
> 总体来说，这个函数实现了在多选项中选择一个的功能，并且支持分页显示选项。
>
