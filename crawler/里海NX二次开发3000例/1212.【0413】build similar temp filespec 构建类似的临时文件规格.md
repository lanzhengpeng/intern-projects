### 【0413】build similar temp filespec 构建类似的临时文件规格

#### 代码

```cpp
    /* This magically works with both native and NXManager mode encoded names! 译:这神奇地适用于原生和NXManager模式编码的名字！ */  
    static void build_similar_temp_filespec(char *fspec, int ftype, char *new_spec)  
    {  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            temp_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            *tmp_dir;  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_CALL(uc4576(fspec, 0, dspec, temp_name));  
        if (strrchr(temp_name, '.')) strcpy(strrchr(temp_name, '.'), "");  
        UF_CALL(uc4575(tmp_dir, ftype, temp_name, new_spec));  
    }

```

#### 代码解析

> 这是NX二次开发代码，用于构建一个类似临时文件规范的字符串。主要功能包括：
>
> 1. 获取NX系统的临时目录路径，并保存在字符串tmp_dir中。
> 2. 使用uc4576函数解析文件路径，提取出目录和文件名，分别保存在dspec和temp_name中。
> 3. 删除文件名中的扩展名（如果存在）。
> 4. 使用uc4575函数，根据文件类型ftype，在临时目录下构建一个符合NX文件命名规范的临时文件路径，并保存在new_spec中。
> 5. 该函数可以处理原生和NXManager模式编码的文件路径，适应性较好。
> 6. 该代码实现了在NX临时目录下，根据给定文件构建一个临时文件路径的功能，适用于NX二次开发中生成临时文件的需求。
>
