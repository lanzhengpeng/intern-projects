### 【1792】prompt for part name 提示输入部件名称

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
    static logical prompt_for_part_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[MAX_FSPEC_SIZE+1],  
            *p_dir,  
            *p_ext;  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_PART_DIR, &p_dir));  
        UF_CALL(UF_UI_ask_dialog_filter(UF_UI_PART_OPEN_FLTR, &p_ext));  
        combine_directory_and_wildcard(p_dir, p_ext, filter);  
        UF_free(p_dir);  
        UF_free(p_ext);  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "Part Name", filter, "", fspec,  
            &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API实现的用于提示用户选择部件文件的函数。主要功能如下：
>
> 1. combine_directory_and_wildcard函数：将用户选择的目录和文件过滤器组合成完整的文件路径。如果用户没有选择目录，则只使用文件过滤器。
> 2. prompt_for_part_name函数：提示用户选择部件文件。首先调用UF_UI_ask_dialog_directory获取用户选择的目录，然后调用UF_UI_ask_dialog_filter获取用户选择的文件过滤器。接着调用combine_directory_and_wildcard组合目录和过滤器，然后调用UF_UI_create_filebox弹出文件选择对话框，让用户选择部件文件。如果用户选择了文件，则将文件路径返回给fspec参数。
> 3. 该函数通过组合用户选择的目录和文件过滤器，实现了灵活的文件选择功能。用户可以根据需要选择目录和过滤器，方便地找到所需的部件文件。
>
