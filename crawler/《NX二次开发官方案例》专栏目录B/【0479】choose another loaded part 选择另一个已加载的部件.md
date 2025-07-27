### 【0479】choose another loaded part 选择另一个已加载的部件

#### 代码

```cpp
    static int ask_all_other_parts(tag_t except_part, tag_t **parts)  
    {  
        int  
            err,  
            ii, jj,  
            n = UF_PART_ask_num_parts();  
        tag_t  
            a_part;  
        if (n > 1)  
        {  
            *parts = UF_allocate_memory((n-1) * sizeof(tag_t), &err);  
            if (UF_CALL(err)) return 0;  
            for (ii = 0, jj = 0; ii < n; ii++)  
            {  
                a_part = UF_PART_ask_nth_part(ii);  
                if (a_part != except_part) (*parts)[jj++] = a_part;  
            }  
        }  
        return n  - 1;  
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
                case 1:                     /* Back 译:Back to the top */  
                    if (a == 0) return -1;  
                    a = a - 13;  
                    break;  
                case 2:                     /* Cancel 译:取消 */  
                    return -1;  
                    break;  
                case 18:                    /* More... 译:更多... */  
                    if ((a + z) < n_choices)  
                    {  
                        a = a + 13;  
                        break;  
                    }  
                default:                    /* Picked one 译:选择一个 */  
                    return a + resp - 5;  
                    break;  
            }  
        } while (TRUE);  
    }  
    static tag_t choose_another_loaded_part(char *prompt)  
    {  
        int  
            ii,  
            n_parts,  
            which;  
        tag_t  
            the_part,  
            this_part = UF_PART_ask_display_part(),  
            *parts;  
        char  
            **leaf_names;  
        n_parts = ask_all_other_parts(this_part, &parts);  
        if (n_parts == 0) return NULL_TAG;  
        if (n_parts == 1)  
        {  
            the_part = parts[0];  
            UF_free(parts);  
            return the_part;  
        }  
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

> 这段代码是NX二次开发中用于选择除当前激活零件外的其他加载零件的函数。主要包含以下步骤：
>
> 1. ask_all_other_parts()：获取除当前激活零件外的所有加载零件的标签数组，并返回零件数量。
> 2. allocate_string_array()：动态分配字符串数组空间，用于存储每个零件的简称。
> 3. ask_part_shortname()：获取指定零件的名称，并生成简称，用于用户选择。
> 4. choose_one_item()：实现选择界面，用于从提供的选项中选择一个，支持翻页和返回等功能。
> 5. choose_another_loaded_part()：主函数，首先获取除当前激活零件外的其他加载零件，然后获取它们的简称，最后调用choose_one_item()让用户选择一个零件，并返回所选零件的标签。
> 6. 在选择过程中，使用NX的交互式对话框来展示零件简称，并让用户通过按键选择。
> 7. 函数内部通过NX的API来获取零件信息，并且使用NX的内存分配函数来动态分配内存。
> 8. 整个函数流程清晰，通过层层调用，最终实现了选择其他加载零件的功能。
>
