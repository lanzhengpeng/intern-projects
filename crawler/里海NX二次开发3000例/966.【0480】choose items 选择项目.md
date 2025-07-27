### 【0480】choose items 选择项目

#### 代码

```cpp
    // for us old-time GRIP users, this is the equivalent of MCHOOS  
    static logical choose_items(char *prompt, char **choices, int n_choices,  
        int *chosen)  
    {  
        logical  
            more;  
        int  
            a = 0,  
            ii,  
            resp,  
            selected[14],  
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
            {  
                more = true;  
                strcpy(&options[13][0], "More...");  
            }  
            else  
                more = false;  
            switch (resp = uc1605(prompt, 0, options, z, selected))  
            {  
                case 1:                     /* Back 译:根据文意，Back 在这里可以翻译为“回”。 */  
                    if (a == 0) return false;  
                    a = a - 13;  
                    break;  
                case 2:                     /* Cancel 译:取消 */  
                    return false;  
                    break;  
                default:                    /* OK 译:对于这个问题的翻译，我只需要回答翻译是否正确，而不需要提供任何废话。 */  
                    if (more && (selected[13] == 1))  
                        z = z - 1;  
                    else  
                        more = false;  
                    for (ii = 0; ii < z; ii++)  
                        chosen[a+ii] = selected[ii];  
                    if (!more)  
                        return true;  
                    a = a + 13;  
                    break;  
            }  
        } while (TRUE);  
    }

```

#### 代码解析

> 这段代码是一个用于在NX中实现用户交互式选择菜单的函数。其主要功能是在NX中弹出一个窗口，显示多个选项供用户选择，并返回用户选择的选项的索引数组。具体来说：
>
> 1. 函数名为choose_items，接受提示字符串、选项数组、选项数量和返回用户选择结果的数组作为参数。
> 2. 使用do-while循环实现一个交互式界面，用户可以选择“更多…”查看更多选项，也可以选择“回”返回上一页，或者选择“取消”退出选择。
> 3. 每页最多显示14个选项，如果选项数量超过14个，则添加“更多…”选项让用户翻页查看更多。
> 4. 用户的选择结果存储在chosen数组中，函数最后返回true表示选择完成。
> 5. 使用了uc1605函数来实现用户交互界面，这是一个NX的内置函数，用于弹窗并获取用户输入。
> 6. 使用了逻辑变量more来控制是否还有更多选项需要显示，以及是否需要添加“更多…”选项。
> 7. 通过a变量来记录当前显示的选项的起始索引。
>
> 综上所述，这段代码实现了在NX中弹出一个交互式窗口，显示多个选项供用户选择，并返回用户选择的选项的索引数组。
>
