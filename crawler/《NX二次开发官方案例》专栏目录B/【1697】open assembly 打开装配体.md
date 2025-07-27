### 【1697】open assembly 打开装配体

#### 代码

```cpp
    static logical open_assembly(char *fspec, tag_t **loaded_parts, int *n_loaded)  
    {  
        int  
            n_new,  
            n_orig;  
        tag_t  
            *new_parts,  
            *orig_parts,  
            part;  
        UF_PART_load_status_t  
            status;  
        *loaded_parts = NULL;  
        *n_loaded = 0;  
        n_orig = ask_parts_in_session(&orig_parts);  
        UF_CALL(UF_PART_open(fspec, &part, &status));  
        if (status.n_parts > 0) report_load_status(&status);  
        if (status.failed == FALSE)  
        {  
            n_new = ask_parts_in_session(&new_parts);  
            *n_loaded = whats_new(orig_parts,n_orig,new_parts,n_new,loaded_parts);  
            UF_free(new_parts);  
            UF_free(orig_parts);  
            return TRUE;  
        }  
        else  
        {  
            UF_free(orig_parts);  
            return FALSE;  
        }  
    }

```

#### 代码解析

> 这是段NX Open C++ API编写的二次开发代码，用于打开NX装配文件，并获取加载的零件信息。
>
> 主要步骤包括：
>
> 1. 获取当前会话中已加载的零件信息，保存到orig_parts数组中，记录数量n_orig。
> 2. 调用UF_PART_open函数打开指定的NX装配文件，并获取加载状态status。
> 3. 如果加载成功，则再次获取当前会话中已加载的零件信息，保存到new_parts数组中，记录数量n_new。
> 4. 比较新旧零件信息，找出新增的零件，保存到loaded_parts数组中，记录数量*n_loaded。
> 5. 释放new_parts和orig_parts数组内存。
> 6. 如果打开文件成功，则返回TRUE；否则返回FALSE。
>
> 整个函数实现了打开NX装配文件，获取加载的零件信息的功能。
>
