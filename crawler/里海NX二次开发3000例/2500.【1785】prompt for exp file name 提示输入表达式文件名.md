### 【1785】prompt for exp file name 提示输入表达式文件名

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
    static logical prompt_for_exp_file_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            *exp_dir,  
            *exp_ext,  
            filter[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_IMPORT_DIR, &exp_dir));  
        UF_CALL(UF_UI_ask_dialog_filter(UF_UI_IMPORT_EXPRESSIONS_FLTR, &exp_ext));  
        combine_directory_and_wildcard(exp_dir, exp_ext, filter);  
        UF_free(exp_dir);  
        UF_free(exp_ext);  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "Expressions File Name", filter, "",  
            fspec, &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
    }

```

#### 代码解析

> 这段NX二次开发代码主要用于实现一个文件对话框，以便用户可以指定一个表达式文件。具体来说：
>
> 1. combine_directory_and_wildcard函数：该函数将目录和通配符组合成完整的文件名。如果用户没有指定目录，则直接使用通配符。如果指定了目录，则在目录后拼接上通配符。
> 2. prompt_for_exp_file_name函数：该函数使用NX的UI接口，弹出一个文件对话框，让用户选择一个表达式文件。对话框的标题和提示信息是可配置的，文件过滤器是从NX配置中获取的。如果用户没有取消对话框，则返回true，并将用户选择的文件名写入到fspec参数中。
> 3. 代码逻辑：首先获取用户指定的目录和文件过滤器，然后组合成完整的文件名。接着弹出文件对话框，让用户选择文件。如果用户没有取消，则返回true并将文件名写入参数。
> 4. NX二次开发：该代码利用NX的UI接口实现了文件对话框，以获取用户指定的表达式文件路径，属于典型的NX二次开发代码。
> 5. 用户交互：该代码通过弹出的对话框与用户进行交互，实现了用户友好性。
>
