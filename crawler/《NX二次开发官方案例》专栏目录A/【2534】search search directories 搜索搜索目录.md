### 【2534】search search directories 搜索搜索目录

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
    static logical search_search_directories(char *short_name, char *full_name)  
    {  
        logical  
            found = FALSE,  
            *sub_dir;  
        int  
            count,  
            ii,  
            type,  
            resp;  
        char  
            **dir_list,  
            fspec[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_ASSEM_ask_search_directories(&count, &dir_list, &sub_dir));  
        for (ii = 0; ((ii < count) && (!found)); ii++)  
        {  
            if (sub_dir[ii])  
            {  
                if ((resp = uc4508(dir_list[ii],(1<<10)|(1<<11)|(1<<13),0,short_name)) < 0)  
                {  
                    UF_CALL(resp);  
                }  
                else  
                {  
                    while ((resp = uc4518()) < 2 && (resp > -1))  
                    {  
                        switch (resp)  
                        {  
                            case 0:  
                                type = uc4601();  
                                if (type == 100)  
                                    uc4509();  
                                else  
                                {  
                                    UF_CALL(uc4519(fspec));  
                                    strcpy(full_name, fspec);  
                                    found = TRUE;  
                                }  
                                break;  
                            case 1:  
                                UF_CALL(uc4549());  
                                break;  
                            default:  
                                UF_CALL(resp);  
                                break;  
                        }  
                    }  
                    UF_CALL(uc4548());  
                }  
            }  
            else  
            {  
                UF_CALL(uc4575(dir_list[ii], 0, short_name, fspec));  
                if (!is_file_there(fspec))  
                {  
                    found = TRUE;  
                    strcpy(full_name, fspec);  
                }  
            }  
        }  
        UF_free_string_array(count, dir_list);  
        UF_free(sub_dir);  
        return found;  
    }

```

#### 代码解析

> 这段NX二次开发代码主要实现了以下功能：
>
> 1. 检查文件是否存在：is_file_there函数用于检查指定的文件是否存在。
> 2. 在NX的搜索目录中查找文件：search_search_directories函数会在NX的搜索目录中查找指定的文件名，并返回文件的完整路径。
> 3. 文件查找流程：该函数首先调用UF_ASSEM_ask_search_directories获取NX的搜索目录列表，然后遍历每个目录。对于每个目录，它会先检查文件是否存在，如果存在，则返回文件路径；如果不存在，它会尝试在目录的子目录中继续查找。
> 4. 子目录查找：在子目录中查找时，函数会使用uc4508打开目录，然后循环调用uc4518遍历目录中的每个文件，并判断是否为普通文件。如果是普通文件，则调用uc4519获取文件名，并返回文件路径。
> 5. 错误处理：函数中包含错误处理，如果中间出现错误，会调用相应的UF函数来处理错误。
> 6. 释放资源：最后，函数会释放之前分配的目录列表和子目录标志数组的内存。
>
> 这段代码通过遍历NX的搜索目录，实现了在NX中查找指定文件名的完整路径的功能。它通过调用NX提供的文件操作API，实现了在NX环境中查找文件的功能。
>
