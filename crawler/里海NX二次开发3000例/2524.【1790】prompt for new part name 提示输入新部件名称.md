### 【1790】prompt for new part name 提示输入新部件名称

#### 代码

```cpp
    static logical is_file_there(char *file_spec)  
    {  
        int missing = TRUE;  
        UF_CALL(UF_CFI_ask_file_exist(file_spec, &missing));  
        if (missing)  
            return FALSE;  
        else  
            return TRUE;  
    }  
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
    static logical prompt_for_new_part_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[UF_CFI_MAX_PATH_NAME_SIZE],  
            *p_dir,  
            *p_ext;  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_PART_DIR, &p_dir));  
        UF_CALL(UF_UI_ask_dialog_filter(UF_UI_PART_OPEN_FLTR, &p_ext));  
        combine_directory_and_wildcard(p_dir, p_ext, filter);  
        UF_free(p_dir);  
        UF_free(p_ext);  
        while (!UF_CALL(UF_UI_create_filebox(prompt, "Part Name", filter, "",  
            fspec, &resp)) && (resp != UF_UI_CANCEL))  
        {  
            if (!is_file_there(fspec)) return TRUE;  
            uc1601("File exists - try again", TRUE);  
        }  
        return FALSE;  
    }

```

#### 代码解析

> 这段NX Open C++代码是用于NX二次开发的示例代码，主要实现以下功能：
>
> 1. 文件存在性检查函数is_file_there：用于检查指定的文件是否存在。通过调用UF_CFI_ask_file_exist函数，并返回布尔值。
> 2. 目录和通配符组合函数combine_directory_and_wildcard：用于将目录和通配符组合成完整的文件路径。它会将目录、通配符和临时文件名拼接，然后替换临时文件名为通配符。
> 3. 提示输入新部件名称函数prompt_for_new_part_name：用于提示用户输入新的部件文件名。它会先获取部件目录和文件过滤条件，然后将目录和过滤条件组合成文件路径。接着，它会不断提示用户输入文件名，直到用户输入一个不存在的文件名。如果用户取消输入，则返回false。该函数主要调用了UF_UI_create_filebox函数。
>
> 这段代码展示了NX Open C++在文件操作和用户交互方面的基本用法，适用于NX二次开发中的文件选择和保存场景。
>
