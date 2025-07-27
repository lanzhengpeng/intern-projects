### 【1286】find in folder 在文件夹中查找

#### 代码

```cpp
    static logical find_in_folder(UF_UGMGR_tag_t folder, char *find_part_number,  
        UF_UGMGR_tag_t *found_in)  
    {  
        int  
            ii,  
            count;  
        UF_UGMGR_tag_t  
            *folder_contents;  
        UF_UGMGR_object_type_t  
            obj_type;  
        char  
            part_number[UF_UGMGR_NAME_SIZE + 1];  
        if (folder == UF_UGMGR_null_tag) return FALSE;  
        UF_CALL(UF_UGMGR_list_folder_contents(folder, &count, &folder_contents));  
        for (ii = 0; ii < count; ii++)  
        {  
            UF_CALL(UF_UGMGR_ask_object_type(folder_contents[ii], &obj_type));  
            switch(obj_type)  
            {  
                case UF_UGMGR_type_folder :  
                    if (find_in_folder(folder_contents[ii], find_part_number,  
                        found_in))  
                    {  
                        UF_free(folder_contents);  
                        return TRUE;  
                    }  
                    break;  
                case UF_UGMGR_type_part :  
                    UF_CALL(UF_UGMGR_ask_part_number(folder_contents[ii],  
                        part_number));  
                    if (!strcmp(part_number, find_part_number))  
                    {  
                        *found_in = folder;  
                        UF_free(folder_contents);  
                        return TRUE;  
                    }  
                    break;  
                default :  
                    break;  
            }  
        }  
        UF_free(folder_contents);  
        return FALSE;  
    }

```

#### 代码解析

> 这段代码是用于NX CAD系统中的二次开发，其主要功能是在NX文件系统中递归搜索一个文件夹，并找到具有指定部件编号的部件。代码逻辑如下：
>
> 1. 定义了一个名为find_in_folder的静态逻辑函数，接收文件夹标签、要搜索的部件编号以及一个输出参数，用于存储找到的部件所在文件夹的标签。
> 2. 首先，检查文件夹标签是否为空，如果是，则返回FALSE。
> 3. 调用UF_UGMGR_list_folder_contents函数，获取文件夹内容列表，包括文件夹和部件对象。
> 4. 遍历文件夹内容列表，对每个对象：获取对象类型。如果是文件夹，递归调用find_in_folder函数，继续搜索该文件夹。如果是部件，获取部件编号，并与要搜索的编号进行比较。如果找到匹配的部件，将所在文件夹标签存储在输出参数中，释放内存，并返回TRUE。
> 5. 获取对象类型。
> 6. 如果是文件夹，递归调用find_in_folder函数，继续搜索该文件夹。
> 7. 如果是部件，获取部件编号，并与要搜索的编号进行比较。
> 8. 如果找到匹配的部件，将所在文件夹标签存储在输出参数中，释放内存，并返回TRUE。
> 9. 如果遍历完所有内容后没有找到匹配的部件，释放内存，返回FALSE。
>
> 这段代码通过递归方式，可以搜索整个NX文件系统，找到具有指定部件编号的部件所在文件夹。
>
