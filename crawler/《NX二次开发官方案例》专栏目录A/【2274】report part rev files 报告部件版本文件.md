### 【2274】report part rev files 报告部件版本文件

#### 代码

```cpp
    static void report_part_rev_files(UF_UGMGR_tag_t revision)  
    {  
        int  
            ii,  
            file_count;  
        char  
            revision_id[UF_UGMGR_NAME_SIZE+1],  
            **file_types,  
            **file_names;  
        UF_CALL(UF_UGMGR_ask_part_revision_id(revision, revision_id));  
        UF_CALL(UF_UGMGR_list_part_rev_files(revision, &file_count,  
            &file_types, &file_names));  
        printf("\tRev %s contains %d non-master file(s).\n", revision_id,  
            file_count);  
        for (ii = 0; ii < file_count; ii++)  
        {  
            printf("\t\t%s - %s\n", file_names[ii],file_types[ii]);  
        }  
        UF_free_string_array(file_count, file_types);  
        UF_free_string_array(file_count, file_names);  
    }

```

#### 代码解析

> 这段代码的主要功能是报告NX部件修订版本中包含的非主文件。具体步骤包括：
>
> 1. 获取当前修订版本的ID，并调用UF_UGMGR_list_part_rev_files函数来获取修订版本中所有非主文件的类型和名称。
> 2. 输出修订版本ID和文件数量，然后遍历文件列表，输出每个文件的名称和类型。
> 3. 释放获取的字符串数组以避免内存泄漏。
> 4. 使用UF_CALL宏确保NX函数调用成功，并在失败时输出错误信息。
> 5. 在释放字符串数组时，使用了UF_free_string_array函数，并传入文件数量作为参数。
> 6. 使用了NX提供的API函数来查询和操作NX部件的修订版本信息。
>
> 总的来说，这段代码实现了查询和报告NX部件修订版本中包含的非主文件的功能。
>
