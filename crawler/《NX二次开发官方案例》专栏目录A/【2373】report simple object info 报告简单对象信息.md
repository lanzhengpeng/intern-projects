### 【2373】report simple object info 报告简单对象信息

#### 代码

```cpp
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #include <uf_assem.h>  
    #define RX(X) report_simple_object_info(#X, X)  
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
            case UF_circle_type: /* non-std format in the header file 译:non-std format in the header file的译文为：头文件中的非标准格式 */  
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
        strcpy(descriptor, "Unknown type");  /* In case its not found 译:如果找不到的话。 */  
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
    static void report_simple_object_info(char *what, tag_t object)  
    {  
        int  
            status,  
            subtype,  
            type;  
        tag_t  
            owner,  
            part = UF_PART_ask_display_part();  
        char  
            name[UF_OBJ_NAME_LEN + 1],  
            msg[133];  
        sprintf(msg, "%s = %d", what, object);  
        ECHO(msg);  
        if (object == NULL_TAG)  
        {  
            ECHO(" (NULL_TAG)\n");  
            return;  
        }  
        status = UF_OBJ_ask_status(object);  
        sprintf(msg, ", status: %d", status);  
        ECHO(msg);  
        if (status == UF_OBJ_DELETED)  
        {  
            ECHO("\n");  
            return;  
        }  
        if (!UF_OBJ_ask_name(object, name))  
        {  
            ECHO(", named: ");  
            ECHO(name);  
        }  
        UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
        ask_object_type_descriptor(type, subtype, msg);  
        ECHO(", type: ");  
        ECHO(msg);  
        if (UF_OBJ_is_object_a_promotion(object))  
            ECHO(", promotion");  
        if (UF_ASSEM_is_occurrence(object))  
        {  
            ECHO(", occurrence");  
            owner = UF_ASSEM_ask_part_occurrence(object);  
            if (owner != UF_ASSEM_ask_root_part_occ(part))  
            {  
                UF_CALL(UF_OBJ_ask_name(owner, msg));  
                ECHO(" (from ");  
                ECHO(msg);  
                ECHO(")");  
            }  
        }  
        ECHO("\n");  
    }

```

#### 代码解析

> 这段代码主要用于NX Open C++中的对象信息查询。
>
> 主要功能包括：
>
> 1. 定义了一个ask_object_type_descriptor函数，用于根据对象类型和子类型，查询对应的类型描述字符串。
> 2. 定义了一个report_simple_object_info函数，用于打印一个NX对象的基本信息，包括对象标签值、状态、名称、类型描述、是否为提升对象、是否为装配体中的实例等。
> 3. report_simple_object_info函数首先调用UF_OBJ_ask_status查询对象状态，如果是已删除状态，则直接打印NULL_TAG并返回。
> 4. 如果对象有效，则接着查询对象名称，并判断是否为提升对象。
> 5. 如果对象是装配体中的实例，则查询其所属组件，并打印组件名称。
> 6. 最后，调用ask_object_type_descriptor函数，查询对象类型描述字符串，并打印出来。
> 7. 在ask_object_type_descriptor函数中，如果是标准类型，则直接返回对应的描述字符串。否则，会打开头文件uf_object_types.h，从里面解析出类型和子类型的描述字符串。
> 8. report_simple_object_info函数提供了宏RX，可以简化调用该函数的代码。
> 9. 这段代码主要用于NX Open C++开发中，打印对象信息，方便调试和了解对象属性。
>
> 总体来说，这段代码实现了NX对象信息的查询和打印，为开发人员提供了便利。
>
