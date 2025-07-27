### 【1784】prompt for existing part name 提示输入现有零件名称

#### 代码

```cpp
    static logical prompt_for_existing_part_name(char *prompt, char *fspec)  
    {  
        logical  
            unused;  
        int  
            resp;  
        UF_CALL(UF_UI_set_prompt(prompt));  
        if (UF_CALL(UF_UI_ask_open_part_filename(fspec, &unused, &resp))  
            || (resp == UF_UI_CANCEL)) return FALSE;  
        return TRUE;  
    }

```

#### 代码解析

> 这段代码是用于NX Open的二次开发，主要实现了以下功能：
>
> 1. 定义了一个名为prompt_for_existing_part_name的静态逻辑函数，用于提示用户输入已经存在的零件名称。
> 2. 函数接收两个参数：prompt和fspec。prompt用于设置提示信息，fspec用于存储用户输入的文件名。
> 3. 在函数内部，定义了一个未使用的逻辑变量unused和一个整数变量resp，分别用于接收UF_UI_set_prompt和UF_UI_ask_open_part_filename的返回值。
> 4. 调用UF_UI_set_prompt函数设置提示信息为prompt。
> 5. 调用UF_UI_ask_open_part_filename函数提示用户输入文件名，并将文件名存储在fspec中。同时获取用户对打开文件的响应，存储在resp中。
> 6. 判断UF_UI_ask_open_part_filename的返回值和resp的值。如果函数调用失败或者用户取消了操作，则返回FALSE。
> 7. 如果函数调用成功且用户没有取消操作，则返回TRUE。
> 8. 通过返回值可以判断用户是否成功输入了文件名。
>
> 综上，这段代码通过NX Open提供的UI接口实现了提示用户输入已经存在的零件名称的功能，返回值用于判断用户是否成功输入了文件名。
>
