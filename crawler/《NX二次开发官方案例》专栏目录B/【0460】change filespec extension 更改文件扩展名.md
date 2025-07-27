### 【0460】change filespec extension 更改文件扩展名

#### 代码

```cpp
    static void build_similar_temp_filespec(char *fspec, int ftype, char *new_spec)  
    {  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            temp_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            *tmp_dir;  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_CALL(uc4576(fspec, 0, dspec, temp_name));  
        if (strrchr(temp_name, '.')) strcpy(strrchr(temp_name, '.'), "");  
        UF_CALL(uc4575(tmp_dir, ftype, temp_name, new_spec));  
    }  
    static void change_filespec_extension(char *fspec, int ftype, char *new_fspec)  
    {  
        logical  
            is_ugmgr;  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            shrt_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_file_name[UF_UGMGR_FNAME_SIZE+1];  
        UF_CALL(UF_is_ugmanager_active(&is_ugmgr));  
        if (!is_ugmgr)  
        {  
            UF_CALL(uc4576(fspec, 0, dspec, shrt_name));  
            UF_CALL(uc4578(shrt_name, -1, shrt_name));  
            UF_CALL(uc4575(dspec, ftype, shrt_name, new_fspec));  
        }  
        else  
        {  
            UF_CALL(UF_UGMGR_decode_part_filename(fspec, part_number,  
                part_revision, part_file_type, part_file_name));  
            build_similar_temp_filespec(part_number, ftype, new_fspec);  
        }  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发，主要包含两个功能：
>
> 1. build_similar_temp_filespec：生成一个临时文件名，用于存储NX文件。它会获取NX的临时目录，截取源文件名去掉扩展名，然后根据文件类型生成临时文件名。
> 2. change_filespec_extension：更改NX文件的扩展名。它首先判断NX是否处于UGManager模式。如果不是，它会截取源文件名去掉扩展名，然后根据文件类型生成新文件名。如果是UGManager模式，它会解析源文件名，获取部件号、版本号等信息，然后调用build_similar_temp_filespec生成临时文件名。
>
> 总的来说，这段代码用于生成临时文件名或更改文件扩展名，是NX二次开发中常用的功能。
>
