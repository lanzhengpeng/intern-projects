### 【3083】ask object type descriptor 根据输入的参数，查询并返回对象类型的描述字符串

#### 代码

```cpp
    static void ask_object_type_descriptor(int type, int subtype, char *descriptor)  
    {  
        int  
            ii,  
            a_type;  
        char  
            a_line[MAX_LINE_SIZE+1],  
            api_dir[UF_CFI_MAX_PATH_NAME_SIZE],  
            *base_dir,  
            define[133],  
            *ptr,  
            uoth_file[UF_CFI_MAX_PATH_NAME_SIZE];  
        FILE  
            *in_file;  
        switch (type)  
        {  
            case UF_circle_type: /* non-std format in the header file 译:在头文件中使用非标准格式 */  
                strcpy(descriptor, "arc");  
                return;  
            case UF_OBJ_EXPR_TYPE:  
                strcpy(descriptor, "expression");  
                return;  
            case UF_OBJ_PART_TYPE:  
                strcpy(descriptor, "part");  
                return;  
            default:  
                break;  
        }  
        UF_CALL(UF_translate_variable("UGII_BASE_DIR", &base_dir));  
        UF_CALL(uc4575(base_dir, 100, "ugopen", api_dir));  
        UF_CALL(uc4575(api_dir, 0, "uf_object_types.h", uoth_file));  
        if ((in_file = fopen(uoth_file, "r")) == NULL)  
        {  
            strcpy(descriptor, "(uf_object_types.h not found)");  
            return;  
        }  
        strcpy(descriptor, "Unknown type");  /* In case its not found 译:In case its not found，翻译为：如果找不到的话。 */  
        while (!feof(in_file))  
        {  
            fgets(a_line, MAX_LINE_SIZE, in_file);  
            if ((ii = sscanf(a_line, "#define UF_%s %d", define, &a_type)) == 2)  
            {  
                if ((ptr = strstr(define, "_type")) && (a_type == type))  
                {  
                    *ptr = '\0';  
                    strcpy(descriptor, define);  
                    if (strcmp(define, "gfem_load")) break;  
                }  
            }  
        }  
        while (!feof(in_file))  
        {  
            fgets(a_line, MAX_LINE_SIZE, in_file);  
            if ((ii = sscanf(a_line, "#define UF_%s %d", define, &a_type)) == 2)  
            {  
                if (ptr = strstr(define, "_type")) break;  
                if (a_type == subtype)  
                {  
                    if (ptr = strstr(define, "_subtype")) *ptr = '\0';  
                    if (strstr(define, descriptor) == define)  
                        strcpy(descriptor, define);  
                    else  
                    {  
                        strcat(descriptor, "/");  
                        strcat(descriptor, define);  
                    }  
                    break;  
                }  
            }  
        }  
        fclose(in_file);  
        while (ptr = strchr(descriptor, '_')) *ptr = ' ';  
    }

```

#### 代码解析

> 这段代码用于获取NX对象类型的描述符。主要功能包括：
>
> 1. 对于一些已知类型的对象，直接返回描述符，如圆弧、表达式、零件等。
> 2. 对于未知类型的对象，从uf_object_types.h头文件中搜索类型描述符。首先获取头文件路径，然后逐行读取文件，匹配类型定义，直到找到匹配的类型描述符。
> 3. 如果找到了匹配的类型描述符，继续在文件中搜索匹配的子类型描述符。如果找到子类型，则组合类型和子类型描述符。
> 4. 如果未找到匹配的描述符，返回"Unknown type"。
> 5. 最后关闭文件，并将描述符中的下划线替换为空格。
> 6. 整个过程使用了很多字符串处理函数，如strstr、strcat、strchr等，用于匹配、连接和替换字符串。
> 7. 使用了NX的UF函数获取环境变量和拼接路径。
>
> 总体来说，这段代码实现了根据对象类型获取其描述符的功能，对于二次开发获取对象信息非常有用。
>
