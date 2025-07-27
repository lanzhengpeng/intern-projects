### 【0410】build copy partname 构建复制零件名称

#### 代码

```cpp
    static void build_copy_partname(char *copy_fspec)  
    {  
        logical  
            tceng;  
        char  
            dspec[MAX_FSPEC_SIZE+1],  
            part_file_name[UF_UGMGR_FNAME_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_fspec[MAX_FSPEC_SIZE+1],  
            part_name[MAX_ENTITY_NAME_SIZE+1],  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            *tmp_dir;  
        tag_t  
            part = UF_PART_ask_display_part();  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(UF_is_ugmanager_active(&tceng));  
        if (!tceng)  
        {  
            UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
            UF_CALL(uc4578(part_name, 2, part_name));  
            strcat(part_name, "-new");  
            UF_CALL(uc4575(dspec, 2, part_name, copy_fspec));  
        }  
        else  
        {  
            UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
            UF_CALL(UF_UGMGR_decode_part_filename(part_fspec, part_number,  
                part_revision, part_file_type, part_file_name));  
            UF_CALL(uc4575(tmp_dir, 2, part_number, copy_fspec));  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，主要功能是构建NX部件的副本文件路径。
>
> 代码的主要步骤如下：
>
> 1. 获取当前显示的部件对象。
> 2. 获取当前部件的文件路径。
> 3. 判断UG Manager是否激活。如果未激活，通过调用内部函数uc4576和uc4578，解析原部件路径，构建新的副本路径，并在末尾加上"-new"后缀。
> 4. 如果UG Manager已激活，通过调用UF_UGMGR_decode_part_filename函数解析原部件路径，获取部件编号、版本等信息。
> 5. 通过调用内部函数uc4575，在UGII_TMP_DIR临时目录下，构建副本路径，并将副本路径作为字符串返回。
> 6. 整个过程使用了NX Open C++ API中的文件路径处理函数，实现了在不同情况下构建合理的部件副本路径。
>
> 总体来说，这段代码提供了在不同环境配置下构建NX部件副本路径的功能，具有一定程度的灵活性。
>
