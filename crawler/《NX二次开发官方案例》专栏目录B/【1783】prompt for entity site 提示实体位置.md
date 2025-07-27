### 【1783】prompt for entity site 提示实体位置

#### 代码

```cpp
    static logical prompt_for_entity_site(char *prompt, int *selection)  
    {  
        int  
            resp;  
        char  
            options[9][38] = { "TOP LEFT",  
                               "TOP CENTER",  
                               "TOP RIGHT",  
                               "MID LEFT",  
                               "MID CENTER",  
                               "MID RIGHT",  
                               "BOTTOM LEFT",  
                               "BOTTOM CENTER",  
                               "BOTTOM RIGHT" };  
        resp = *selection;  
        resp = uc1603(prompt, resp, options, 9);  
        if ((resp > 4) && (resp < 19))  
        {  
            *selection = resp - 4;  
            return TRUE;  
        }  
        else return FALSE;  
    }

```

#### 代码解析

> 这段代码是用于NX Open的二次开发，其功能是在图形界面上弹出一个选择框，让用户从9个选项中选择一个，并将用户的选择存储在变量selection中。
>
> 具体步骤如下：
>
> 1. 定义了一个名为prompt_for_entity_site的静态函数，该函数接受两个参数：prompt用于提示信息，selection用于存储用户的选择。
> 2. 定义了9个选项，存储在options数组中。
> 3. 获取用户的选择，首先尝试从selection中获取，如果没有则提示用户选择。
> 4. 用户选择后，将选择的索引值存储在selection中，然后返回TRUE表示成功。
> 5. 如果用户取消选择，则返回FALSE表示失败。
> 6. 对用户选择的索引值进行了处理，将其映射到0-8的范围内，以对应9个选项。
> 7. 该函数可以被其他代码调用，以实现让用户从9个位置中选择一个的目的。
>
> 总体而言，这段代码通过弹出选择框的方式，实现了让用户在图形界面上进行选择的功能，并将用户的选择存储在selection变量中，可以被其他代码使用。
>
