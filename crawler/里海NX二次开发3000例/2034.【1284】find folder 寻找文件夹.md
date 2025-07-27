### 【1284】find folder 寻找文件夹

#### 代码

```cpp
    static UF_UGMGR_tag_t find_folder(UF_UGMGR_tag_t look_in, char *find_name)  
    {  
        int  
            ii,  
            count;  
        UF_UGMGR_tag_t  
            found_folder = UF_UGMGR_null_tag,  
            *folder_contents;  
        UF_UGMGR_object_type_t  
            obj_type;  
        char  
            this_name[UF_UGMGR_NAME_SIZE + 1];  
        UF_CALL(UF_UGMGR_list_folder_contents(look_in, &count, &folder_contents));  
        for (ii = 0; ii < count; ii++)  
        {  
            UF_CALL(UF_UGMGR_ask_object_type(folder_contents[ii], &obj_type));  
            if (obj_type == UF_UGMGR_type_folder)  
            {  
                UF_CALL(UF_UGMGR_ask_folder_name(folder_contents[ii], this_name));  
                if (!strcmp(this_name, find_name))  
                {  
                    found_folder = folder_contents[ii];  
                    break;  
                }  
                found_folder = find_folder(folder_contents[ii], find_name);  
                if (found_folder != UF_UGMGR_null_tag) break;  
            }  
        }  
        UF_free(folder_contents);  
        return found_folder;  
    }

```

#### 代码解析

> 这段代码实现了在NX CAD系统中的文件夹中递归查找指定名称的文件夹的功能。
>
> 主要步骤包括：
>
> 1. 调用UF_UGMGR_list_folder_contents函数获取指定文件夹的内容，包括子文件夹和文件。
> 2. 遍历文件夹内容，使用UF_UGMGR_ask_object_type判断每个对象是文件夹还是文件。
> 3. 如果是文件夹，使用UF_UGMGR_ask_folder_name获取文件夹名称。
> 4. 比较文件夹名称是否与要查找的名称一致，一致则返回该文件夹句柄。
> 5. 如果不一致，递归调用find_folder函数在子文件夹中查找。
> 6. 如果找到，则返回文件夹句柄；否则返回null表示未找到。
> 7. 最后释放内存。
>
> 通过递归调用，这段代码可以递归地在所有子文件夹中查找指定名称的文件夹，实现了NX CAD系统中的文件夹递归查找功能。
>
