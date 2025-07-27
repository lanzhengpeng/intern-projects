### 【2595】select a printer profile 选择打印机配置文件

#### 代码

```cpp
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
                case 2:                     /* Cancel 译:Cancel */  
                    return -1;  
                    break;  
                case 18:                    /* More... 译:More...翻译为：更多... */  
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
    static void select_a_printer_profile(char *printer, char *profile)  
    {  
        int  
            ii,  
            n_groups,  
            n_printers,  
            n_profiles,  
            printer_n,  
            profile_n;  
        char  
            *job_name,  
            **group_names,  
            **printers,  
            **profiles;  
        UF_PLOT_printer_group_t  
            *printer_groups;  
        UF_CALL(UF_PLOT_ask_printer_groups(&n_groups, &printer_groups));  
        if (n_groups > 1)  
        {  
            UF_CALL(allocate_string_array(n_groups, UF_PLOT_MAX_STRING_SIZE,  
                &group_names));  
            for (ii = 0; ii < n_groups; ii++)  
                strcpy(group_names[ii], printer_groups[ii].label);  
            ii = choose_one_item("Select Printer Group", group_names, n_groups);  
            if (ii !=  -1)  
            {  
                UF_CALL(UF_PLOT_set_printer_group(printer_groups[ii].group_dir,  
                    printer_groups[ii].jobs_dir,  
                    printer_groups[ii].home_dir));  
            }  
            UF_free_string_array(n_groups, group_names);  
        }  
        if (n_groups > 0) UF_free(printer_groups);  
        UF_CALL(UF_PLOT_ask_printer_names(&n_printers, &printers));  
        if (n_printers == 0)  
        {  
            uc1601("No printers found.", TRUE);  
            return;  
        }  
        if (n_printers > 1)  
            printer_n = choose_one_item("Select Printer", printers, n_printers);  
        else  
            printer_n = 0;  
        UF_CALL(UF_PLOT_ask_profile_names(printers[printer_n], &n_profiles,  
            &profiles));  
        if (n_profiles > 1)  
            profile_n = choose_one_item("Select Profile", profiles, n_profiles);  
        else  
            profile_n = 0;  
        strcpy(printer, printers[printer_n]);  
        strcpy(profile, profiles[profile_n]);  
        UF_free_string_array(n_printers, printers);  
        UF_free_string_array(n_profiles, profiles);  
    }

```

#### 代码解析

> 这段代码是NX CAD软件的二次开发代码，主要用于选择打印机和配置文件。
>
> 1. 主要函数介绍：allocate_string_array：动态分配字符串数组，用于存储打印机组名称、打印机名称和配置文件名称。choose_one_item：从多个选项中选择一个，用于选择打印机组、打印机和配置文件。select_a_printer_profile：主函数，用于选择打印机和配置文件。
> 2. allocate_string_array：动态分配字符串数组，用于存储打印机组名称、打印机名称和配置文件名称。
> 3. choose_one_item：从多个选项中选择一个，用于选择打印机组、打印机和配置文件。
> 4. select_a_printer_profile：主函数，用于选择打印机和配置文件。
> 5. 工作流程：获取打印机组信息，如果存在多个，则让用户选择一个。获取打印机列表，如果存在多个，则让用户选择一个。获取选定打印机的配置文件列表，如果存在多个，则让用户选择一个。最后将选定的打印机和配置文件名称返回。
> 6. 获取打印机组信息，如果存在多个，则让用户选择一个。
> 7. 获取打印机列表，如果存在多个，则让用户选择一个。
> 8. 获取选定打印机的配置文件列表，如果存在多个，则让用户选择一个。
> 9. 最后将选定的打印机和配置文件名称返回。
> 10. 代码特点：使用了NX提供的UF_PLOT接口来获取打印机和配置文件信息。使用了内存动态分配来存储字符串数组。使用了循环和条件判断来处理各种情况。使用了用户交互函数来让用户做出选择。
> 11. 使用了NX提供的UF_PLOT接口来获取打印机和配置文件信息。
> 12. 使用了内存动态分配来存储字符串数组。
> 13. 使用了循环和条件判断来处理各种情况。
> 14. 使用了用户交互函数来让用户做出选择。
> 15. 需要注意的问题：需要释放分配的内存，避免内存泄露。需要处理用户取消选择的情况。需要处理获取信息失败的情况。
> 16. 需要释放分配的内存，避免内存泄露。
> 17. 需要处理用户取消选择的情况。
> 18. 需要处理获取信息失败的情况。
>
> 综上所述，这段代码实现了在NX中通过用户交互选择打印机和配置文件的功能。
>
