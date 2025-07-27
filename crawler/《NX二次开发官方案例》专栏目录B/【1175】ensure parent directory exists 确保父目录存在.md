### 【1175】ensure parent directory exists 确保父目录存在

#### 代码

```cpp
    static void ensure_directory_exists(char *dir)  
    {  
        char  
            folder[UF_CFI_MAX_FILE_NAME_SIZE],  
            parent[UF_CFI_MAX_PATH_NAME_SIZE];  
        if (uc4560(dir, 100))  
        {  
            UF_CALL(uc4576(dir, 100, parent, folder));  
            ensure_directory_exists(parent);  
            UF_CALL(uc4563(dir, 100));  
        }  
    }  
    static void ensure_parent_directory_exists(char *dir)  
    {  
        char  
            filename[UF_CFI_MAX_FILE_NAME_SIZE],  
            parent[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_CALL(uc4576(dir, 0, parent, filename));  
        ensure_directory_exists(parent);  
    }

```

#### 代码解析

> 根据代码，这是用于在NX Open C++中创建目录的代码片段。主要功能如下：
>
> 1. ensure_directory_exists函数：检查指定目录是否存在，如果不存在，则创建该目录。首先会调用uc4560函数检查目录是否存在，如果不存在，则通过uc4576函数获取父目录和文件名，然后递归调用自身以创建父目录，最后调用uc4563函数创建目录。
> 2. ensure_parent_directory_exists函数：检查指定路径的父目录是否存在，如果不存在，则创建父目录。该函数通过uc4576函数获取父目录路径，然后调用ensure_directory_exists函数来创建父目录。
>
> 代码通过递归调用自身来创建目录及其所有祖先目录，从而确保指定的目录存在。这是NX Open C++中实现目录创建的一种常见做法。
>
