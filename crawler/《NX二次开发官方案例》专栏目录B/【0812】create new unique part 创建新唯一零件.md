### 【0812】create new unique part 创建新唯一零件

#### 代码

```cpp
    static void build_unique_temp_part_name(char *fspec)  
    {  
        logical  
            tceng;  
        char  
            *tmp_dir,  
            unique[MAX_FSPEC_SIZE+1];  
        UF_CALL(uc4577(unique));  
        UF_CALL(UF_is_ugmanager_active(&tceng));  
        if (tceng)  
            UF_CALL(UF_UGMGR_encode_part_filename(unique, "A", "", "", fspec));  
        else  
        {  
            UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
            UF_CALL(uc4575(tmp_dir, 2, unique, fspec));  
        }  
    }  
    static tag_t create_new_unique_part(void)  
    {  
        int  
            units;  
        tag_t  
            new_part,  
            old_part = UF_PART_ask_display_part();  
        char  
            unique[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_PART_ask_units(old_part, &units));  
        build_unique_temp_part_name(unique);  
        UF_CALL(UF_PART_new(unique, units, &new_part));  
        return new_part;  
    }

```

#### 代码解析

> 这段代码是用于在NX中创建一个具有唯一名称的新部件的函数。
>
> 主要包含两个函数：
>
> 1. build_unique_temp_part_name(char *fspec) - 构建一个唯一的临时部件名称，将结果存储在fspec指向的字符串中。
> 2. create_new_unique_part() - 创建一个具有唯一名称的新部件。
>
> 具体步骤如下：
>
> 生成的唯一名称是利用NX提供的函数生成的，确保在当前会话中是唯一的。
>
> 总的来说，这段代码实现了在NX中创建一个具有唯一名称的新部件的功能，可供NX二次开发时使用。
>
