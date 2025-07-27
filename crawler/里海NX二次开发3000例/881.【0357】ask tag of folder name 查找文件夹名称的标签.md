### 【0357】ask tag of folder name 查找文件夹名称的标签

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
    static UF_UGMGR_tag_t ask_tag_of_folder_name(char *folder_name)  
    {  
        UF_UGMGR_tag_t  
            root_folder;  
        char  
            root_name[UF_UGMGR_NAME_SIZE + 1] = { "" };  
        UF_CALL(UF_UGMGR_ask_root_folder(&root_folder));  
        UF_CALL(UF_UGMGR_ask_folder_name(root_folder, root_name));  
        if (!strcmp(root_name, folder_name)) return root_folder;  
        return find_folder(root_folder, folder_name);  
    }

```

#### 代码解析

> 这段代码是NX OpenUF的二次开发代码，主要实现了以下功能：
>
> 1. find_folder函数：递归地在NX的UG Manager文件夹结构中查找指定名称的文件夹，并返回该文件夹的tag。
> 2. ask_tag_of_folder_name函数：首先获取NX根文件夹的tag，并判断其名称是否与指定名称一致。如果一致，则返回根文件夹的tag；否则，调用find_folder函数在根文件夹下递归查找指定名称的文件夹，并返回该文件夹的tag。
> 3. 代码逻辑：通过UF_UGMGR_list_folder_contents、UF_UGMGR_ask_object_type、UF_UGMGR_ask_folder_name等UF API函数，实现了对UG Manager文件夹结构的遍历和查询，最终找到并返回指定名称的文件夹的tag。
> 4. 代码结构：采用递归结构实现了文件夹的递归遍历，同时采用了UF提供的API函数进行NX UG Manager的操作，整体代码结构清晰，功能完整。
> 5. 代码应用：这段代码可用于NX二次开发中，根据文件夹名称获取其对应的tag，进而进行后续操作，如打开、删除等。
>
