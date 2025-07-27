### 【0723】create batch shade 创建批处理渲染

#### 代码

```cpp
    static void build_unique_temp_name(char *fspec, int ftype)  
    {  
        char  
            *tmp_dir,  
            unique[UF_CFI_MAX_FILE_NAME_SIZE];  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_CALL(uc4577(unique));  
        UF_CALL(uc4575(tmp_dir, ftype, unique, fspec));  
    }  
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
    /*  UF_ABORT_ask_flag_status does not work with UF_DISP_batch_shade_options  
        See PR 5766077.  This function demonstrates a work around.  
        Return:  
            TRUE = Batch shade file was created successfully  
            FALSE = Process failed or was aborted by the user  
    */  
    static logical create_batch_shade(char* filename, int x_size, int y_size,  
        UF_DISP_shade_method_t method,  UF_DISP_shade_options_p_t options)  
    {  
        int  
            missing;  
        char  
            file_ext[5],  
            temp_spec[UF_CFI_MAX_PATH_NAME_SIZE],  
            uniq_spec[UF_CFI_MAX_PATH_NAME_SIZE];  
        strcpy(file_ext, strrchr(filename, '.'));  
        build_unique_temp_name(uniq_spec, 0);  
        strcat(uniq_spec, file_ext);  
        UF_CALL(UF_CFI_ask_file_exist(uniq_spec, &missing));  
        if (!missing) uc4561(uniq_spec, 0);  
        if (UF_CALL(UF_DISP_batch_shade_options(uniq_spec, x_size, y_size,  
            method, options))) return FALSE;  
        build_similar_temp_filespec(filename, 0, temp_spec);  
        strcat(temp_spec, file_ext);  
        uc4567(uniq_spec, temp_spec, UF_CFI_MOVE_ALWAYS_REPLACE, 0, 0);  
        UF_CALL(UF_CFI_ask_file_exist(uniq_spec, &missing));  
        if (missing)  
        {  
            uc4567(temp_spec, filename, UF_CFI_MOVE_ALWAYS_REPLACE, 0, 0);  
            return TRUE;  
        }  
        else  
        {  
            uc4561(temp_spec, 0);  
            return FALSE;  
        }  
    }

```

#### 代码解析

> 这段代码是用于NX CAD系统的二次开发，主要实现创建批处理阴影渲染文件的功能。
>
> 代码中主要实现了以下几个功能：
>
> 1. 构建临时文件名：使用函数build_unique_temp_name构建一个唯一的临时文件名，以便后续创建批处理阴影文件。
> 2. 构建类似的临时文件名：使用函数build_similar_temp_filespec根据原始文件名构建一个类似的临时文件名，用于后续生成批处理阴影文件。
> 3. 创建批处理阴影文件：函数create_batch_shade用于创建批处理阴影文件。首先，它会检查临时文件名是否已存在，如果存在则删除。然后，调用NX的UF_DISP_batch_shade_options函数来创建批处理阴影文件。如果创建成功，它会将临时文件移动到目标文件名，并返回TRUE。如果创建失败，则返回FALSE。
>
> 总体来说，这段代码通过构建临时文件名、创建批处理阴影文件，并将临时文件移动到目标文件，实现了NX CAD系统下创建批处理阴影渲染文件的功能。
>
