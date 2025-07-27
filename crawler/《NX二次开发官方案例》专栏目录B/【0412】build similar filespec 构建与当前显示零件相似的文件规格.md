### 【0412】build similar filespec 构建与当前显示零件相似的文件规格

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
    static void build_similar_filespec(char *fspec, int ftype)  
    {  
        logical  
            is_ugmgr;  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_file_name[UF_UGMGR_FNAME_SIZE+1];  
        UF_CALL(UF_is_ugmanager_active(&is_ugmgr));  
        UF_PART_ask_part_name(part, part_fspec);  
        if (!is_ugmgr)  
        {  
            UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
            UF_CALL(uc4578(part_name, 2, part_name));  
            UF_CALL(uc4575(dspec, ftype, part_name, fspec));  
        }  
        else  
        {  
            UF_CALL(UF_UGMGR_decode_part_filename(part_fspec, part_number,  
                part_revision, part_file_type, part_file_name));  
            build_similar_temp_filespec(part_number, ftype, fspec);  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的二次开发代码，主要用于构建与当前打开的NX部件相似的临时文件路径。
>
> 主要功能包括：
>
> 1. 提取文件路径：首先，通过调用UF_PART_ask_display_part()获取当前显示的部件，然后调用UF_PART_ask_part_name()获取部件的完整路径。
> 2. 检查UGManager是否激活：接着，通过调用UF_is_ugmanager_active()检查UGManager是否激活。如果未激活，则通过调用uc4576()和uc4578()提取部件名称和目录，然后调用uc4575()构建相似路径。
> 3. 构建临时文件路径：如果UGManager激活，则调用UF_UGMGR_decode_part_filename()解析部件文件名，获取部件号、版本、文件类型和文件名。然后调用build_similar_temp_filespec()函数，传递部件号和文件类型，构建与部件相似的临时文件路径。
> 4. 处理文件扩展名：在build_similar_temp_filespec()函数中，会处理文件扩展名，删除点号和后面的扩展名，以确保生成的临时文件路径不包含扩展名。
> 5. 构建临时文件路径：最后，调用uc4575()函数，传递临时目录、文件类型和文件名，生成与部件相似的临时文件路径。
>
> 总体来说，这段代码的目的是为了构建一个与当前打开的NX部件相似的临时文件路径，以便进行一些与该部件相关的二次开发操作。
>
