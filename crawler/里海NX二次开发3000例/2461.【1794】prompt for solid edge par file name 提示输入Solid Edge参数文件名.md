### 【1794】prompt for solid edge par file name 提示输入Solid Edge参数文件名

#### 代码

```cpp
    static logical prompt_for_solid_edge_par_file_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[UF_CFI_MAX_PATH_NAME_SIZE] = { "*.par" };  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "Solid Edge File", filter, "",  
            fspec, &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
    }

```

#### 代码解析

> 这段代码是用于在NX Open UI中创建一个文件对话框，提示用户选择一个Solid Edge参数文件(par)的函数。
>
> 代码的主要功能包括：
>
> 1. 定义了一个静态逻辑函数prompt_for_solid_edge_par_file_name，用于在NX Open UI中创建一个文件对话框，提示用户选择一个Solid Edge参数文件(par)。
> 2. 函数接受两个参数：prompt表示对话框的提示信息，fspec表示用户选择的文件路径字符串。
> 3. 定义了一个filter字符串数组，用于指定文件过滤器，这里只允许选择扩展名为par的文件。
> 4. 调用UF_UI_create_filebox函数创建文件对话框，参数包括提示信息、对话框标题、文件过滤器、默认文件名、用户选择的文件路径、以及用户响应状态。
> 5. 如果创建文件对话框成功，并且用户响应不是取消，则函数返回TRUE，表示用户选择了文件。如果创建失败或用户取消，则返回FALSE。
> 6. 该函数可以在NX Open UI应用程序中调用，以方便用户选择Solid Edge参数文件。
>
> 综上所述，这段代码的主要功能是在NX Open UI中创建一个文件对话框，提示用户选择一个Solid Edge参数文件(par)。
>
