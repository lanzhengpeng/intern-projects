### 【0368】ask ugmgr part tag 问UG管理器部件标签

#### 代码

```cpp
    static UF_UGMGR_tag_t ask_ugmgr_part_tag(tag_t part)  
    {  
        char  
            encoded_name[MAX_FSPEC_SIZE + 1],  
            part_file_name[UF_UGMGR_FNAME_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1];  
        UF_UGMGR_tag_t  
            db_part_tag;  
        UF_PART_ask_part_name(part, encoded_name);  
        UF_CALL(UF_UGMGR_decode_part_filename(encoded_name, part_number,  
            part_revision, part_file_type, part_file_name));  
        UF_CALL(UF_UGMGR_ask_part_tag(part_number, &db_part_tag));  
        return db_part_tag;  
    }

```

#### 代码解析

> 这段NX二次开发代码的功能是查询NX部件库中指定部件的数据库标签。具体步骤如下：
>
> 1. 使用UF_PART_ask_part_name()函数获取当前打开的NX部件的名称，并存储在encoded_name数组中。
> 2. 使用UF_UGMGR_decode_part_filename()函数解析encoded_name中的部件信息，包括部件号(part_number)、部件版本(part_revision)、文件类型(part_file_type)和文件名(part_file_name)。
> 3. 使用UF_UGMGR_ask_part_tag()函数，根据部件号(part_number)查询NX部件库，获取该部件在数据库中的标签(db_part_tag)。
> 4. 最后，函数返回查询到的db_part_tag。
>
> 总体来说，这段代码实现了从NX部件名称解析出部件号，然后根据部件号查询到该部件在NX部件库中的数据库标签的功能。这对于需要访问NX部件库数据的应用场景非常有用。
>
