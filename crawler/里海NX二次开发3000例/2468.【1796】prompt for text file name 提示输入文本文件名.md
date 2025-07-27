### 【1796】prompt for text file name 提示输入文本文件名

#### 代码

```cpp
    static logical prompt_for_text_file_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[MAX_FSPEC_SIZE+1] = { "*.txt" };  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "Text File Name", filter, "",  
            fspec, &resp)) && (resp != UF_UI_CANCEL))  
        {  
            if (!strstr(fspec, ".txt")) strcat(fspec, ".txt");  
            WRITENZ(uc4573(fspec, 4, fspec));  
            return TRUE;  
        }  
        else return FALSE;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，用于创建一个对话框，提示用户输入文本文件名。
>
> 代码的主要步骤如下：
>
> 1. 定义一个静态函数prompt_for_text_file_name，用于提示用户输入文本文件名。函数接收两个参数，prompt为提示信息，fspec为用户输入的文件名。
> 2. 定义一个文件过滤器filter，只允许用户选择后缀为.txt的文本文件。
> 3. 使用UF_UI_create_filebox函数创建一个文件对话框，让用户选择或输入文件名。如果用户选择了文件并点击确定，则将文件名保存到fspec中，并返回响应码。
> 4. 如果用户没有取消对话框，并且文件名不以.txt结尾，则在文件名后添加.txt后缀。
> 5. 将文件名写入日志文件。
> 6. 如果用户没有取消对话框，则返回true，表示成功获取了文件名。如果用户取消了对话框，则返回false。
>
> 这段代码通过文件对话框让用户选择或输入文本文件名，并自动添加.txt后缀，实现了提示用户输入文本文件名的功能。
>
