### 【1789】prompt for jt file name 提示输入JT文件名

#### 代码

```cpp
    static void combine_directory_and_wildcard(char *dir, char *fltr, char *spec)  
    {  
        if (!strcmp(dir, ""))  
            strcpy(spec, fltr);  
        else  
        {  
            UF_CALL(uc4575(dir, 0, "junk.xxx", spec));  
            strcpy(strstr(spec, "junk.xxx"), fltr);  
        }  
    }  
    static logical prompt_for_jt_file_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[MAX_FSPEC_SIZE+1],  
            *p_dir;  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_PART_DIR, &p_dir));  
        combine_directory_and_wildcard(p_dir, "*.jt", filter);  
        UF_free(p_dir);  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "JT File Name", filter, "",  
            fspec, &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
    }

```

#### 代码解析

> 这段代码是用于NX Open的二次开发，其主要功能是创建一个文件对话框，允许用户选择一个JT文件。
>
> 代码的主要流程如下：
>
> 1. 获取默认目录：首先调用UF_UI_ask_dialog_directory获取NX的默认零件目录。
> 2. 组合目录和通配符：将默认目录和通配符".jt"组合成文件过滤条件，以便后续在文件对话框中使用。
> 3. 创建文件对话框：调用UF_UI_create_filebox创建一个文件对话框，用户可以在其中选择文件。其中，prompt是提示信息，"JT File Name"是对话框的标题，filter是文件过滤条件，fspec用于存储用户选择的文件路径。
> 4. 获取用户响应：对话框关闭后，通过UF_UI_create_filebox的返回值和响应码判断用户是否选择了文件。如果选择了文件，则将文件路径存储在fspec中，并返回true；如果用户取消了对话框，则返回false。
> 5. 释放内存：使用UF_free释放获取默认目录时分配的内存。
> 6. 返回结果：最终返回用户是否选择了文件的逻辑值。
>
> 总体来说，这段代码实现了创建文件对话框并获取用户选择的JT文件的功能。
>
