### 【1259】file exists 文件是否存在

#### 代码

```cpp
    static logical file_exists(char *file_spec)  
    {  
        int  
            in_session,  
            missing;  
        UF_CALL(UF_CFI_ask_file_exist(file_spec, &missing));  
        in_session = UF_PART_is_loaded(file_spec);  
        if (missing && (in_session != 1) && (in_session != 2))  
            return FALSE;  
        else  
            return TRUE;  
    }

```

#### 代码解析

> 这段NX二次开发代码定义了一个名为file_exists的静态逻辑函数，用于检查指定的文件是否存在。
>
> 函数的主要逻辑如下：
>
> 1. 使用UF_CFI_ask_file_exist函数检查文件是否存在，若不存在则missing变量为1。
> 2. 使用UF_PART_is_loaded函数检查文件是否已加载到当前NX会话中。in_session变量取值为0表示未加载，1表示已加载，2表示部分加载。
> 3. 如果文件不存在，且in_session不为1或2，表示文件既不存在，也没有加载到会话中，此时函数返回FALSE。
> 4. 如果文件存在，或者文件已加载到会话中，函数返回TRUE。
>
> 因此，该函数通过检查文件是否存在以及是否加载到当前NX会话，来判断文件是否存在，从而实现了文件存在性的检查功能。
>
