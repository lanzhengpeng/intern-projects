### 【0411】build new partname 构建新的零件名称

#### 代码

```cpp
    static void build_new_partname(char *old_fspec, char *addon, char *copy_fspec)  
    {  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE];  
        UF_CALL(uc4576(old_fspec, 2, dspec, part_name));  
        UF_CALL(uc4578(part_name, 2, part_name));  
        strcat(part_name, addon);  
        UF_CALL(uc4575(dspec, 2, part_name, copy_fspec));  
    }

```

#### 代码解析

> 这段代码是用于在NX Open中构建新零件名称的函数，其主要功能如下：
>
> 1. 调用uc4576函数，从旧的零件文件路径中提取目录路径和原始零件名称，分别存入dspec和part_name数组。
> 2. 调用uc4578函数，去除原始零件名称的扩展名，得到不带扩展名的零件名称。
> 3. 将新的后缀addon拼接到不带扩展名的零件名称后面，得到新的零件名称。
> 4. 调用uc4575函数，将目录路径和新的零件名称拼接成完整的新零件文件路径，存入copy_fspec数组。
> 5. 这个函数通过NX Open API实现了从旧零件文件路径构建新零件文件路径的功能，适用于NX二次开发中的文件操作场景。
>
