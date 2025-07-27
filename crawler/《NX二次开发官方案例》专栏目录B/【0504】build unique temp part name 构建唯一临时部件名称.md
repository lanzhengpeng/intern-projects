### 【0504】build unique temp part name 构建唯一临时部件名称

#### 代码

```cpp
    static void build_unique_temp_part_name(char *fspec)  
    {  
        logical  
            tceng;  
        char  
            *tmp_dir,  
            unique[UF_CFI_MAX_FILE_NAME_SIZE];  
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

```

#### 代码解析

> 这段代码的主要功能是构建一个唯一的临时文件名，用于NX的二次开发。
>
> 代码的主要逻辑如下：
>
> 1. 调用函数uc4577生成一个唯一的字符串，长度为UF_CFI_MAX_FILE_NAME_SIZE。
> 2. 判断UGManager是否激活。如果是，则调用UF_UGMGR_encode_part_filename函数，传入生成的唯一字符串和"A"作为参数，生成临时文件名并保存到fspec中。
> 3. 如果UGManager没有激活，则调用UF_translate_variable获取UGII_TMP_DIR变量值，即临时目录的路径。
> 4. 调用函数uc4575，传入临时目录路径、2、生成的唯一字符串，生成临时文件名并保存到fspec中。
>
> 通过以上步骤，代码成功构建了一个唯一的临时文件名，用于NX的二次开发。这个文件名既考虑了UGManager是否激活，也考虑了临时目录的路径，保证了文件名的唯一性。
>
