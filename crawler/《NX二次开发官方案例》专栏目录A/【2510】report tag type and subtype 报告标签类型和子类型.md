### 【2510】report tag type and subtype 报告标签类型和子类型

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
        strcpy(descriptor, "Unknown type");  /* In case its not found 译:如果找不到，请提供翻译。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_Tt(X) report_tag_type_and_subtype(#X, X)  
    static void report_tag_type_and_subtype(char *name, tag_t object)  
    {  
        int  
            subtype,  
            type;  
        char  
            desc[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        if (object != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            ask_object_type_descriptor(type, subtype, desc);  
            sprintf(msg,"%s = %d, type = %d, subtype = %d (%s)\n", name, object,   
                type, subtype, desc);  
        }  
        else   
            sprintf(msg, "%s = %d (NULL_TAG)\n", name, object);  
        WRITE(msg);  
    }

```

#### 代码解析

> 这是段NX二次开发代码，其主要功能是查询NX对象的对象类型和子类型，并将其输出到NX的Listing窗口中。
>
> 代码的核心是report_tag_type_and_subtype函数。该函数接受一个NX对象的tag和名称作为参数，首先使用UF_OBJ_ask_type_and_subtype函数查询该对象的对象类型和子类型。然后调用ask_object_type_descriptor函数，根据对象类型和子类型查询到描述字符串。最后，将对象的tag、类型、子类型和描述字符串拼接成字符串，并输出到NX的Listing窗口中。
>
> ask_object_type_descriptor函数通过读取NX安装目录下的uf_object_types.h文件，根据对象类型和子类型找到对应的描述字符串。对于一些标准类型，如圆弧、表达式等，直接返回固定的描述字符串。
>
> 通过#define定义了WRITE和WRITE_Tt两个宏，用于打开NX的Listing窗口并输出信息，简化了代码。
>
> 整体来说，这段代码实现了查询并输出NX对象类型和子类型的功能，通过读取系统头文件获取类型描述，利用宏简化输出代码，具有较好的可读性和可维护性。
>
