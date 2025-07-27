### 【0164】ask configured rev 询问配置版本

#### 代码

```cpp
    static logical ask_configured_rev(char *part_number, char *revision_id)  
    {  
        UF_UGMGR_tag_t  
            database_part_tag,  
            part_revision;  
        strcpy(revision_id, "");  
        if (UF_CALL(UF_UGMGR_ask_part_tag(part_number, &database_part_tag)) ||  
            UF_CALL(UF_UGMGR_ask_configured_rev(database_part_tag,  
                &part_revision)) ||  
            UF_CALL(UF_UGMGR_ask_part_revision_id(part_revision, revision_id)))  
            return FALSE;  
        return TRUE;  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发，旨在查询指定部件的已配置修订版本号。其主要功能包括：
>
> 1. 定义了函数ask_configured_rev，用于查询指定部件的已配置修订版本号。
> 2. 通过UF_UGMGR_ask_part_tag函数，根据部件编号获取部件的标签。
> 3. 使用UF_UGMGR_ask_configured_rev函数，根据部件标签获取已配置的修订版本标签。
> 4. 通过UF_UGMGR_ask_part_revision_id函数，根据修订版本标签获取修订版本号。
> 5. 如果查询过程中出现错误，函数将返回FALSE，否则返回TRUE。
> 6. 在函数内部，使用strcpy函数将获取的修订版本号复制到传入的revision_id参数中。
> 7. 函数通过NX的UF函数进行二次开发，实现了查询指定部件的已配置修订版本号的功能。
>
> 总的来说，这段代码的作用是实现了NX中查询指定部件已配置修订版本号的功能。
>
