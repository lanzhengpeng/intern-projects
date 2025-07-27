### 【0437】ask true or false 询问真或假

#### 代码

```cpp
    static logical ask_true_or_false(char *prompt, logical *deflt)  
    {  
        int  
            resp;  
        char  
            options[2][38] = { "True", "False" };  
        resp = 2 - *deflt;  
        resp = uc1603(prompt, resp, options, 2);  
        if ((resp > 4) && (resp < 19))  
        {  
            *deflt = 6 - resp;  
            return TRUE;  
        }  
        else return FALSE;  
    }

```

#### 代码解析

> 这段代码是一个逻辑询问函数，主要用于询问用户一个问题，并获取用户输入的True或False作为返回结果。其主要功能如下：
>
> 1. 函数名为ask_true_or_false，接受三个参数：提示信息prompt、默认值deflt以及返回值。
> 2. 内部定义了选项数组options，包含True和False两个选项。
> 3. 根据默认值deflt，初始化响应值resp为2-deflt，即默认为True时resp为1，默认为False时resp为0。
> 4. 调用uc1603函数向用户显示提示信息并获取用户的选择，该函数返回用户选择的索引。
> 5. 根据用户的选择，更新deflt的值。如果用户选择True,则deflt变为True(1)，如果选择False,则deflt变为False(0)。
> 6. 如果用户进行了有效选择(选择了True或False)，则返回TRUE。否则返回FALSE。
> 7. 该函数通过提示用户选择True或False来获取用户输入，并根据用户的选择更新默认值deflt。返回值表示用户是否进行了有效选择。
>
> 这个函数通过交互方式获取用户对True或False的选择，可用于各种需要用户输入True或False的场景，并通过更新deflt来记忆用户的选择。
>
