### 【1566】locate pattern part 查找图案部件

#### 代码

```cpp
    static logical is_file_there(char *file_spec)  
    {  
        int missing = TRUE;  
        UF_CALL(UF_CFI_ask_file_exist(file_spec, &missing));  
        if (missing)  
            return FALSE;  
        else  
            return TRUE;  
    }  
    static logical locate_pattern_part(char *shortname, char *fullpath)  
    {  
        logical  
            is_ugmgr;  
        char  
            *pdir;  
        strcpy(fullpath, shortname);  
        if (is_file_there(fullpath)) return TRUE;  
        UF_CALL(UF_is_ugmanager_active(&is_ugmgr));  
        if (!is_ugmgr)  
        {  
            UF_CALL(UF_translate_variable("UGII_PATDIR", &pdir));  
            UF_CALL(uc4575(pdir, 2, shortname, fullpath));  
            if (is_file_there(fullpath)) return TRUE;  
            UF_CALL(UF_translate_variable("UGII_UTIL", &pdir));  
            UF_CALL(uc4575(pdir, 2, shortname, fullpath));  
            if (is_file_there(fullpath)) return TRUE;  
        }  
        strcpy(fullpath, "");  
        return FALSE;  
    }

```

#### 代码解析

> 这段代码是用于在NX CAD系统中检查文件是否存在的二次开发代码。主要功能包括：
>
> 1. 检查文件是否存在：函数is_file_there()接受文件路径作为参数，并调用UF_CFI_ask_file_exist()函数来检查该文件是否存在。如果文件不存在，返回FALSE；如果文件存在，返回TRUE。
> 2. 查找文件全路径：函数locate_pattern_part()接受文件简称和全路径作为参数。首先，它尝试使用文件简称作为全路径来检查文件是否存在。如果不存在，它将尝试在UGII_PATDIR和UGII_UTIL目录下查找文件。如果找到文件，返回TRUE并更新全路径；如果找不到，返回FALSE并将全路径清空。
> 3. 使用NX提供的API函数：代码中使用了NX提供的API函数，如UF_CFI_ask_file_exist()和UF_is_ugmanager_active()等，以实现文件检查和路径查找的功能。
> 4. 实现NX环境下文件查找的逻辑：代码实现了NX环境下查找文件全路径的逻辑，考虑了文件可能存在于多个目录的情况，并通过NX提供的API函数进行实现。
>
> 总体来说，这段代码的主要作用是检查文件是否存在，并在NX环境下查找文件的全路径。通过使用NX提供的API函数，实现了对NX文件系统的访问和操作。
>
