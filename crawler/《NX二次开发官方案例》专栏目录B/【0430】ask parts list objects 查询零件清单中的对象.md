### 【0430】ask parts list objects 查询零件清单中的对象

#### 代码

```cpp
    /*HEAD ASK_PARTS_LIST_OBJECTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_plist.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，V18中新增了UF_print_syslog函数。这是一个打印系统日志的函数。该函数的具体功能是将指定的日志消息打印到系统日志文件中。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
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
        strcpy(descriptor, "Unknown type");  /* In case its not found 译:如果找不到，请只回答翻译，不要废话。 */  
        while (!feof(in_file))  
        {  
            fgets(a_line, MAX_LINE_SIZE, in_file);  
            if ((ii = sscanf(a_line, "#define UF_%s %d", define, &a_type)) == 2)  
            {  
                if ((ptr = strstr(define, "_type")) && (a_type == type))  
                {  
                    *ptr = '\0';  
                    strcpy(descriptor, define);  
                    break;  
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
    static void do_it(void)  
    {  
        int  
            ii,  
            n = 0;  
        tag_t  
            *objects;  
        char  
            msg[133],  
            obj_name[UF_OBJ_NAME_LEN + 1];  
        UF_CALL(UF_PLIST_ask_objects(UF_PLIST_ask_tag(), &n, &objects));  
        sprintf(msg, "%d items in Parts List\n", n);  
        WRITE(msg);  
        for (ii = 0; ii < n; ii++)  
        {  
            if (UF_OBJ_ask_name(objects[ii], obj_name))  
            {  
                strcpy(obj_name, "(NoName)");  
            }  
            report_tag_type_and_subtype(obj_name, objects[ii]);  
        }  
        if( n > 0 ) UF_free( objects );  
    }  
    /* 里海 */  
    extern void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这是段NX的二次开发代码，主要功能是遍历零件清单中的所有对象，并输出每个对象的类型和子类型。
>
> 主要流程包括：
>
> 1. 初始化NX环境
> 2. 获取当前零件清单的tag
> 3. 询问零件清单中的对象数量和对象数组
> 4. 遍历对象数组，输出每个对象的名称、tag、类型和子类型
> 5. 释放对象数组内存
> 6. 结束NX环境
>
> 在询问对象类型和子类型时，会首先尝试从uf_object_types.h头文件中获取对象描述信息，如果无法找到，则输出"Unknown type"。在输出时，会首先尝试获取对象名称，如果没有名称，则输出"(NoName)"。
>
> 整个流程通过UF_PLIST_ask_objects和UF_OBJ_ask_type_and_subtype等NX提供的API来实现。此外，还定义了UF_CALL宏来简化错误处理，并定义了report_error函数来打印错误信息。
>
