### 【0481】choose loaded part 选择已加载的部件

#### 代码

```cpp
    static int ask_all_parts(tag_t **parts)  
    {  
        int  
            err,  
            ii,  
            n = UF_PART_ask_num_parts();  
        if (n > 0)  
        {  
            *parts = UF_allocate_memory(n * sizeof(tag_t), &err);  
            if (UF_CALL(err)) return 0;  
            for (ii = 0; ii < n; ii++)  
            {  
                (*parts)[ii] = UF_PART_ask_nth_part(ii);  
            }  
        }  
        return n;  
    }  
    static int allocate_string_array(int n_elements, int n_chars, char ***where)  
    {  
        int  
            ii,  
            resp;  
        *where = UF_allocate_memory(n_elements * sizeof(char *), &resp);  
        for (ii = 0; ii < n_elements; ii++)  
        {  
            (*where)[ii] = UF_allocate_memory(n_chars * sizeof(char), &resp);  
            if (resp) return resp;  
        }  
        return 0;  
    }  
    static void ask_part_shortname(tag_t part, char *shortname)  
    {  
        logical  
            nxman;  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_PART_ask_part_name(part, partname));  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        if (nxman)  
            UF_CALL(UF_PART_name_for_display(partname, shortname));  
        else  
            UF_CALL(uc4574(partname, 2, shortname));  
    }  
    static int choose_one_item(char *prompt, char **choices, int n_choices)  
    {  
        int  
            a = 0,  
            ii,  
            resp,  
            z;  
        char  
            options[14][38];  
        do  
        {  
            z = (n_choices - a < 14) ? n_choices - a : 14;  
            for (ii = 0; ii < z; ii++)  
            {  
                strncpy(&options[ii][0], choices[a+ii], 37);  
                options[ii][37] = '\0';  
            }  
            if ((z == 14) && ((a + z) < n_choices))  
                strcpy(&options[13][0], "More...");  
            switch (resp = uc1603(prompt, 0, options, z))  
            {  
                case 1:                     /* Back 译:Back */  
                    if (a == 0) return -1;  
                    a = a - 13;  
                    break;  
                case 2:                     /* Cancel 译:Cancel的翻译为：取消 */  
                    return -1;  
                    break;  
                case 18:                    /* More... 译:翻译为：更多... */  
                    if ((a + z) < n_choices)  
                    {  
                        a = a + 13;  
                        break;  
                    }  
                default:                    /* Picked one 译:根据提供的原文，翻译为“选中一个”。 */  
                    return a + resp - 5;  
                    break;  
            }  
        } while (TRUE);  
    }  
    static tag_t choose_loaded_part(char *prompt)  
    {  
        int  
            ii,  
            n_parts,  
            which;  
        tag_t  
            the_part,  
            *parts;  
        char  
            **leaf_names;  
        n_parts = ask_all_parts(&parts);  
        UF_CALL(allocate_string_array(n_parts, UF_CFI_MAX_FILE_NAME_LEN,  
            &leaf_names));  
        for (ii = 0; ii < n_parts; ii++)  
            ask_part_shortname(parts[ii], leaf_names[ii]);  
        if ((which = choose_one_item(prompt, leaf_names, n_parts)) == -1)  
            the_part = NULL_TAG;  
        else  
            the_part = parts[which];  
        UF_free(parts);  
        UF_free_string_array(n_parts, leaf_names);  
        return the_part;  
    }

```

#### 代码解析

> 这段NX二次开发代码主要包含以下几个部分：
>
> 1. 获取所有部件：ask_all_parts函数通过UF_PART_ask_num_parts获取当前打开的NX部件数量，然后使用UF_PART_ask_nth_part获取每个部件的tag。
> 2. 动态分配字符串数组：allocate_string_array函数用于动态分配一个二维字符串数组，用于存储部件名称。
> 3. 获取部件简称：ask_part_shortname函数通过UF_PART_ask_part_name获取部件的全名，然后调用UF_PART_name_for_display或uc4574函数获取简称。
> 4. 选择一个条目：choose_one_item函数用于在弹出的对话框中显示多个选项，用户可以选择一个，并返回选择的索引。
> 5. 选择一个已加载的部件：choose_loaded_part函数获取所有部件，然后调用choose_one_item函数让用户选择一个部件，并返回被选中的部件的tag。
> 6. 释放内存：在choose_loaded_part函数的最后，释放了获取部件时分配的内存。
>
> 总体来说，这段代码主要用于在NX中获取当前打开的所有部件，并让用户选择一个，然后返回该部件的tag。
>
