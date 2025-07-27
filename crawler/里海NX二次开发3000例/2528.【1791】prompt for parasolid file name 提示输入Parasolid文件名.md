### 【1791】prompt for parasolid file name 提示输入Parasolid文件名

#### 代码

```cpp
    static logical prompt_for_parasolid_file_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[MAX_FSPEC_SIZE+1] = { "*.x_t" };  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "Parasolid Text File",  
            filter, "", fspec, &resp)) && (resp != UF_UI_CANCEL))  
            return TRUE;  
        else  
            return FALSE;  
    }

```

#### 代码解析

> 这是一段用于在NX中创建文件对话框的二次开发代码。其主要功能是向用户展示一个文件对话框，用于选择Parasolid格式的文本文件。
>
> 代码的主要部分包括：
>
> 1. 定义了一个名为prompt_for_parasolid_file_name的静态逻辑函数，用于打开文件对话框并获取用户选择的文件路径。
> 2. 函数接受两个参数，prompt是提示信息，fspec用于存储用户选择的文件路径。
> 3. 定义了一个过滤字符串filter，表示只显示扩展名为.x_t的文件。
> 4. 调用NX的UF_UI_create_filebox函数来创建文件对话框，用户可以在对话框中选择文件。
> 5. 如果用户没有取消，并且文件对话框返回成功，则将选择的文件路径存储在fspec中，并返回TRUE。
> 6. 如果用户取消或文件对话框返回失败，则返回FALSE。
> 7. 该函数可以方便地嵌入到NX的二次开发代码中，用于让用户选择Parasolid格式的文本文件。
>
