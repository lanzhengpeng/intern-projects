### 【1173】ensure directory exists 确保目录存在

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

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，其功能是确保指定的目录存在。具体来说，该函数首先检查输入的目录路径是否超出了最大长度限制，如果未超过，它会将目录路径分解为父目录路径和子目录名称。接着，递归调用自身来确保父目录存在。最后，使用NX Open C++ API函数创建子目录。总体来看，该代码通过递归调用实现了目录创建的功能，确保了指定的目录路径存在。
>
