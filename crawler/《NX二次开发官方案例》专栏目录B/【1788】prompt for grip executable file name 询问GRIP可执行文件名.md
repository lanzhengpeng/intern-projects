### 【1788】prompt for grip executable file name 询问GRIP可执行文件名

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
    static logical prompt_for_grip_executable_file_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            *grip_dir,  
            *grip_ext,  
            filter[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_GRIP_DIR, &grip_dir));  
        UF_CALL(UF_UI_ask_dialog_filter(UF_UI_FILE_GRIP_FLTR, &grip_ext));  
        combine_directory_and_wildcard(grip_dir, grip_ext, filter);  
        UF_free(grip_dir);  
        UF_free(grip_ext);  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "GRIP Executable", filter, "",  
            fspec, &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
    }

```

#### 代码解析

> 这段代码是NX二次开发代码，主要功能是提示用户输入GRIP可执行文件名，并检查其有效性。
>
> 关键步骤包括：
>
> 1. 调用UF_UI_ask_dialog_directory()获取GRIP可执行文件所在的目录。
> 2. 调用UF_UI_ask_dialog_filter()获取GRIP可执行文件的文件名过滤器。
> 3. 将目录和过滤器拼接成一个文件名通配符。
> 4. 调用UF_UI_create_filebox()弹出文件选择对话框，提示用户选择GRIP可执行文件。如果用户选择了一个文件并点击确定，则将文件名复制到fspec中，函数返回true；如果用户取消，则返回false。
> 5. 使用UF_free()释放临时字符串的内存。
> 6. combine_directory_and_wildcard()函数用于拼接目录和过滤器。
> 7. prompt_for_grip_executable_file_name()函数用于提示用户输入文件名，并检查其有效性。
>
> 总的来说，这段代码实现了让用户输入GRIP可执行文件名的功能，并对输入的文件名进行了有效性检查。
>
