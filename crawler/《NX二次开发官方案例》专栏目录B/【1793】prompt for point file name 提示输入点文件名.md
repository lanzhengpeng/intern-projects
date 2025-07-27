### 【1793】prompt for point file name 提示输入点文件名

#### 代码

```cpp
    static logical prompt_for_point_file_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[UF_CFI_MAX_PATH_NAME_SIZE] = { "*.dat" };  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "Point File", filter, "",  
            fspec, &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
    }

```

#### 代码解析

> 这是NX二次开发中的一个对话框函数，其主要功能是提示用户输入点文件名并返回点文件路径。
>
> 具体代码实现步骤如下：
>
> 1. 定义函数prompt_for_point_file_name，接收提示信息prompt和文件名变量fspec作为参数。
> 2. 设置文件过滤条件filter为"*.dat"，表示只显示后缀为.dat的文件。
> 3. 使用UF_UI_create_filebox函数创建一个文件对话框，并设置对话框的标题为"Point File"，过滤条件为filter，默认文件名为空，文件名变量为fspec。
> 4. 用户在对话框中选择文件后，根据返回值resp判断用户操作。如果用户没有取消，则返回TRUE表示成功获取文件路径；如果用户取消，则返回FALSE。
> 5. 最后，函数返回获取文件路径的成功或失败标志。
>
> 该函数通过UF_UI_create_filebox函数创建一个简单的文件对话框，用户可以在对话框中选择一个后缀为.dat的点文件，并将文件路径返回给调用者。
>
