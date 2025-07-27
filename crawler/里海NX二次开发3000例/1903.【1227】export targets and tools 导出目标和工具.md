### 【1227】export targets and tools 导出目标和工具

#### 代码

```cpp
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void build_unique_temp_part_name(char *fspec)  
    {  
        char  
            *tmp_dir,  
            unique[UF_CFI_MAX_FILE_NAME_SIZE];  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_CALL(uc4577(unique));  
        UF_CALL(uc4575(tmp_dir, 2, unique, fspec));  
    }  
    static void export_targets_and_tools(int n_targets, tag_t *targets, int  
        n_tools, tag_t *tools)  
    {  
        int  
            ii;  
        char  
            obj_name[MAX_ENTITY_NAME_SIZE+1],  
            tmp_prt[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_PART_export_options_t  
            options = { TRUE, UF_PART_maintain_params, UF_PART_copy_exp_deeply };  
        WRITE_D(n_targets);  
        for (ii = 0; ii < n_targets; ii++)  
        {  
            sprintf(obj_name, "TARGET_%d", ii);  
            UF_CALL(UF_OBJ_set_name(targets[ii], obj_name));  
        }  
        WRITE_D(n_tools);  
        for (ii = 0; ii < n_tools; ii++)  
        {  
            sprintf(obj_name, "TOOL_%d", ii);  
            UF_CALL(UF_OBJ_set_name(tools[ii], obj_name));  
        }  
        build_unique_temp_part_name(tmp_prt);  
        WRITE("Exporting to ");  
        WRITE_S(tmp_prt);  
        UF_CALL(UF_PART_export_with_options(tmp_prt, n_targets, targets, &options));  
        options.new_part = FALSE;  
        UF_CALL(UF_PART_export_with_options(tmp_prt, n_tools, tools, &options));  
    }

```

#### 代码解析

> 这段代码是用于NX（Siemens NX）的二次开发代码，主要功能是导出目标体和工具。
>
> 1. 首先，代码定义了两个宏WRITE和WRITE_D，分别用于输出字符串和整数到NX的listing窗口。
> 2. 接着定义了两个函数write_integer_to_listing_window和write_string_to_listing_window，用于向listing窗口输出整数和字符串。
> 3. build_unique_temp_part_name函数用于构建一个唯一的临时部件文件名。
> 4. export_targets_and_tools函数是核心功能，用于导出目标体和工具。它首先获取目标体和工具的数量，并输出到listing窗口。然后为每个目标体和工具设置一个唯一的名称。接着调用build_unique_temp_part_name获取一个唯一的临时文件路径，并将目标体和工具导出到该路径。
> 5. 导出目标体时，使用了UF_PART_export_with_options函数，其中参数maintain_params设置为true，表示保持参数化特征。
> 6. 导出工具时，将maintain_params设置为false，表示不保持参数化特征。
> 7. 最后，函数成功导出了目标体和工具，并将相关信息输出到listing窗口。
>
> 总的来说，这段代码实现了NX中目标体和工具的导出功能，并通过宏和函数定义实现了信息输出和临时文件名的构建，提高了代码的复用性和可维护性。
>
