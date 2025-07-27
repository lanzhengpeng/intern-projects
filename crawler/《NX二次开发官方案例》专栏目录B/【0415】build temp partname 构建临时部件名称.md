### 【0415】build temp partname 构建临时部件名称

#### 代码

```cpp
    static void build_temp_partname(char *copy_fspec)  
    {  
        logical  
            tceng;  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            *tmp_dir;  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(UF_is_ugmanager_active(&tceng));  
        if (tceng)  
        {  
            UF_CALL(UF_UGMGR_decode_part_filename(part_fspec, part_name, dspec,  
                dspec, dspec));  
        }  
        else  
        {  
            UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
            UF_CALL(uc4578(part_name, 2, part_name));  
        }  
        strcat(part_name, "-tmp");  
        UF_CALL(uc4575(tmp_dir, 2, part_name, copy_fspec));  
    }

```

#### 代码解析

> 这段代码的目的是为了在NX中构建一个临时部件名称，主要步骤包括：
>
> 1. 获取当前显示的部件对象。
> 2. 获取临时目录路径。
> 3. 根据当前是否在UGManager中运行，使用不同的函数解码部件名称和目录。
> 4. 将解码后的部件名称添加"-tmp"后缀。
> 5. 将临时目录和部件名称组合成完整的临时文件路径。
> 6. 将完整的临时文件路径复制到输入参数copy_fspec中。
>
> 主要逻辑是构建一个包含临时目录和带后缀的部件名称的路径，并将其作为结果返回。
>
