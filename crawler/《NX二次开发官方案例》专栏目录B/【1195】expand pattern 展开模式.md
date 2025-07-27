### 【1195】expand pattern 展开模式

#### 代码

```cpp
    static logical is_file_there(char *file_spec)  
    {  
        int missing = TRUE;  
        UF_CALL(UF_CFI_ask_file_exist(file_spec, &missing));  
        if (missing)  
            return FALSE;  
        else  
            return TRUE;  
    }  
    static void expand_pattern(tag_t pattern)  
    {  
        int  
            ii,  
            layer,  
            n_members,  
            old_layer,  
            status,  
            view_dep;  
        tag_t  
            group,  
            *members,  
            view_tag;  
        double  
            data[13];  
        char  
            ffn[UF_CFI_MAX_PATH_NAME_SIZE],  
            name[MAX_ENTITY_NAME_SIZE+1],  
            *pdir,  
            sfn[UF_CFI_MAX_PATH_NAME_SIZE],  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_import_part_modes_t  
            modes = { 0, 1, 0, 0, 0, FALSE, FALSE };  
        uc5822(pattern, name, sfn, &view_dep, &layer, &status, data);  
    /*  Try to find the required pattern part 译:尝试找到所需的模式部分。 */  
        strcpy(ffn, sfn);  
        if (!is_file_there(ffn))  
        {  
            UF_CALL(UF_translate_variable("UGII_PATDIR", &pdir));  
            UF_CALL(uc4575(pdir, 2, sfn, ffn));  
        }  
        if (!is_file_there(ffn))  
        {  
            UF_CALL(UF_translate_variable("UGII_UTIL", &pdir));  
            UF_CALL(uc4575(pdir, 2, sfn, ffn));  
        }  
        if (!is_file_there(ffn)) return;  
    /*  Set the work layer 译:设置工作层 */  
        UF_CALL(UF_LAYER_ask_work_layer(&old_layer));  
        UF_CALL(UF_LAYER_set_status(layer, UF_LAYER_WORK_LAYER));  
    /*  Merge in the pattern part 译:Merge in the pattern part。 */  
        UF_CALL(UF_PART_import(ffn, &modes, data, &data[9], data[12], &group));  
    /*  Name the group with the same name the pattern originally had 译:将原名相同的组命名为原名。 */  
        UF_CALL(UF_OBJ_set_name(group, name));  
    /*  Convert all geometry to view dependent 译:将所有几何图形转换为视图相关。 */  
        if (view_dep)  
        {  
            uc6409(pattern, &view_dep, view_name);  
            UF_CALL(UF_VIEW_ask_tag_of_view_name(view_name, &view_tag));  
            UF_CALL(UF_GROUP_ask_group_data(group, &members, &n_members));  
            for (ii = 0; ii < n_members; ii++)  
                UF_CALL(UF_VIEW_convert_to_view(view_tag, members[ii]));  
            if (n_members > 0) UF_free(members);  
        }  
    /*  Reset the work layer 译:重置工作层 */  
        UF_CALL(UF_LAYER_set_status(old_layer, UF_LAYER_WORK_LAYER));  
    /*  Delete the original pattern object 译:删除原始图案对象。 */  
        UF_CALL(UF_OBJ_delete_object(pattern));  
    }

```

#### 代码解析

> 这段NX Open C++代码用于展开NX装配中的组件模式。
>
> 主要步骤包括：
>
> 1. 检查模式组件的文件是否存在，并尝试在多个目录中查找。
> 2. 设置工作层为模式层。
> 3. 导入模式组件，并创建一个新组。
> 4. 将新组重命名为模式组件的原始名称。
> 5. 如果模式是视图相关的，将组内所有几何体转换为视图相关。
> 6. 重置工作层为原始的工作层。
> 7. 删除原始模式组件对象。
>
> 通过这段代码，可以方便地将NX装配中的模式组件展开为实际的几何体，并进行后续操作。代码考虑了文件路径、视图相关性等因素，以确保模式组件能正确展开。
>
