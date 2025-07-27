### 【1781】prompt for cgm file name 提示输入CGM文件名

#### 代码

```cpp
    static void combine_directory_and_wildcard(char *dir, char *fltr, char *spec)  
    {  
        if (!strcmp(dir, ""))  
            strcpy(spec, fltr);  
        else  
        {  
            UF_CALL(uc4575(dir, 0, "junk.xxx", spec));  
            strcpy(strstr(spec, "junk.xxx"), fltr);  
        }  
    }  
    static logical prompt_for_cgm_file_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            *cgm_dir,  
            *cgm_ext,  
            filter[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_IMPORT_DIR, &cgm_dir));  
        UF_CALL(UF_UI_ask_dialog_filter(UF_UI_IMPORT_CGM_FLTR, &cgm_ext));  
        combine_directory_and_wildcard(cgm_dir, cgm_ext, filter);  
        UF_free(cgm_dir);  
        UF_free(cgm_ext);  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "CGM File Name", filter, "",  
            fspec, &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
    }

```

#### 代码解析

> 这段代码是NX Open UI的二次开发代码，其主要功能是提示用户选择一个CGM格式的文件。
>
> 具体步骤包括：
>
> 1. 获取NX默认的导入目录和CGM文件过滤器。
> 2. 将目录和过滤器组合成完整的文件路径。
> 3. 弹出文件选择对话框，提示用户选择一个CGM文件。
> 4. 用户选择文件后，将文件路径存储在fspec参数中。
> 5. 返回逻辑值，表示用户是否成功选择了文件。
> 6. 使用UF_UI函数进行NX UI的二次开发。
> 7. 使用UF_CALL宏封装UF函数调用，用于错误处理。
> 8. 使用UF_free释放动态分配的内存。
> 9. 使用strcmp和strcpy等标准C函数。
> 10. 使用strstr查找子字符串。
>
> 总的来说，这段代码通过NX UI的二次开发实现了选择CGM文件的功能，采用了NX提供的UF函数和标准C函数，并且进行了错误处理和内存管理。
>
