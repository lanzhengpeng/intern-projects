### 【1302】find specification of master 找到主部件的规范

#### 代码

```cpp
    static logical find_specification_of_master(tag_t master, char *encoded_spec)  
    {  
        logical  
            active,  
            found = FALSE;  
        int  
            ii,  
            jj,  
            file_count,  
            revision_count;  
         tag_t  
            a_part;  
         UF_UGMGR_tag_t  
            db_part_tag,  
            *revisions;  
        char  
            a_part_file_name[UF_UGMGR_FNAME_SIZE+1],  
            a_part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            a_part_number[UF_UGMGR_PARTNO_SIZE+1],  
            a_part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            encoded_name[MAX_FSPEC_SIZE + 1],  
            **file_types,  
            **file_names,  
            part_file_name[UF_UGMGR_FNAME_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            revision_id[UF_UGMGR_NAME_SIZE+1];  
        UF_CALL(UF_is_ugmanager_active(&active));  
        if (!active) return FALSE;  
        UF_PART_ask_part_name(master, encoded_name);  
        UF_CALL(UF_UGMGR_decode_part_filename(encoded_name, part_number,  
            part_revision, part_file_type, part_file_name));  
    /*  First check for specification already open in session 译:首先检查会话中是否已打开规范。 */  
        file_count = UF_PART_ask_num_parts();  
        for (ii = 0; ii < file_count; ii++)  
        {  
            a_part = UF_PART_ask_nth_part(ii);  
            UF_PART_ask_part_name(a_part, encoded_spec);  
            UF_CALL(UF_UGMGR_decode_part_filename(encoded_spec, a_part_number,  
                a_part_revision, a_part_file_type, a_part_file_name));  
            if (!strcmp(part_number, a_part_number) &&  
                !strcmp(part_revision, a_part_revision) &&  
                !strcmp(a_part_file_type, "specification"))  
                    return TRUE;  
        }  
    /*  Second check database for specification 译:第二次检查数据库的规格说明书。 */  
        UF_CALL(UF_UGMGR_ask_part_tag(part_number, &db_part_tag));  
        UF_CALL(UF_UGMGR_list_part_revisions(db_part_tag, &revision_count,  
            &revisions));  
        for (ii = 0; ii < revision_count; ii++)  
        {  
            UF_CALL(UF_UGMGR_ask_part_revision_id(revisions[ii], revision_id));  
            if (strcmp(revision_id, part_revision)) continue;  
            UF_CALL(UF_UGMGR_list_part_rev_files(revisions[ii], &file_count,  
                &file_types, &file_names));  
            for (jj = 0; jj < file_count; jj++)  
            {  
                if (strcmp(file_types[jj], "specification")) continue;  
                UF_CALL(UF_UGMGR_encode_part_filename(part_number,  
                    part_revision, file_types[jj], file_names[jj], encoded_spec));  
                found = TRUE;  
                break;  
            }  
            UF_free_string_array(file_count, file_types);  
            UF_free_string_array(file_count, file_names);  
            break;  
        }  
        UF_free(revisions);  
        return found;  
    }

```

#### 代码解析

> 这段代码是用于在NX环境中查找指定零件的主规格说明书的函数。主要步骤如下：
>
> 1. 检查UGManager是否激活，若未激活则返回FALSE。
> 2. 获取指定零件的编号、版本、文件类型和文件名。
> 3. 遍历当前会话中的所有打开零件，检查是否存在与指定零件同编号和版本的主规格书，若找到则返回TRUE。
> 4. 如果会话中没有找到，则从数据库中获取该零件的所有版本信息。
> 5. 遍历每个版本，检查其文件列表中是否存在主规格书文件，若找到则返回TRUE。
> 6. 如果数据库中也没有找到，则返回FALSE。
> 7. 在遍历过程中，使用UF_UGMGR系列函数进行文件名编解码。
> 8. 使用UF_PART系列函数获取零件信息。
> 9. 使用动态内存分配获取版本和文件列表信息，并在使用后释放。
>
> 该函数实现了在NX环境中查找指定零件的主规格说明书的功能，通过会话和数据库的双重检查来确保找到正确的文件。
>
