### 【1780】prompt for any existing part name 提示输入任何现有零件名称

#### 代码

```cpp
    static logical pick_one_of_two_choices(char *prompt, char *option_one,  
        char *option_two, int *choice)  
    {  
        int  
            resp;  
        char  
            options[2][38];  
        strncpy(&options[0][0], option_one, 37);  
        strncpy(&options[1][0], option_two, 37);  
        options[0][37] = '\0';  
        options[1][37] = '\0';  
        resp = uc1603(prompt, *choice, options, 2);  
        if ((resp > 4) && (resp < 19))  
        {  
            *choice = resp - 4;  /* returns 1 or 2 译:根据上下文，returns 1 or 2 翻译为“返回1或2”。 */  
            return TRUE;  
        }  
        else return FALSE;  
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
    static logical prompt_for_any_existing_part_name(char *prompt, char *fspec)  
    {  
        logical  
            nxman,  
            unused;  
        int  
            mode = 1,  
            resp;  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        if (nxman)  
        {  
            if (pick_one_of_two_choices(prompt, "in Native", "in TcEng",  
                &mode))  
            {  
                if (mode == 1)  
                    return (prompt_for_part_name(prompt, fspec));  
                else  
                {  
                    UF_CALL(UF_UI_set_prompt(prompt));  
                    if (UF_CALL(UF_UI_ask_open_part_filename(fspec, &unused, &resp))  
                        || (resp == UF_UI_CANCEL)) return FALSE;  
                    return TRUE;  
                }  
            }  
            else return FALSE;  
        }  
        else  
        {  
            UF_CALL(UF_UI_set_prompt(prompt));  
            if (UF_CALL(UF_UI_ask_open_part_filename(fspec, &unused, &resp))  
                || (resp == UF_UI_CANCEL)) return FALSE;  
            return TRUE;  
        }  
    }

```

#### 代码解析

> 这段代码是NX二次开发中的用户界面交互代码，主要实现了以下功能：
>
> 1. 选择对话框：使用pick_one_of_two_choices函数，弹出一个对话框，让用户在两个选项中选择一个，并将选择结果返回给调用者。
> 2. 组合目录和通配符：使用combine_directory_and_wildcard函数，将目录和文件过滤条件组合成一个完整的文件规格字符串。
> 3. 提示输入部件名称：使用prompt_for_part_name函数，提示用户输入部件名称，并返回完整的文件路径。
> 4. 提示选择已有部件名称：使用prompt_for_any_existing_part_name函数，首先判断NX Manager是否激活，如果激活，则让用户选择是在Native还是TcEng模式下打开部件，然后在相应模式下提示用户输入或选择已有部件名称。
> 5. 调用NX提供的用户界面函数：使用了NX提供的用户界面函数，如UF_UI_create_filebox、UF_UI_ask_dialog_directory、UF_UI_ask_dialog_filter等，来实现文件选择、目录选择和过滤条件选择等功能。
> 6. 错误处理：使用了UF_CALL宏来简化NX函数调用的错误处理。
>
> 总体来说，这段代码实现了在NX二次开发中与用户交互，获取用户输入或选择的功能。
>
