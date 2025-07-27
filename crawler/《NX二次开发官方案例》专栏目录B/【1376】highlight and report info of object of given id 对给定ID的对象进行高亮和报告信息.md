### 【1376】highlight and report info of object of given id 对给定ID的对象进行高亮和报告信息

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    static logical prompt_for_an_integer(char *prompt, char *item, int *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        int  
            da[1];  
        strncpy(&menu[0][0], item, 15);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
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
            case UF_circle_type: /* non-std format in the header file 里海译:在头文件中使用非标准格式 */  
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
        strcpy(descriptor, "Unknown type");  /* In case its not found 里海译:如果找不到，请这样回答。 */  
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
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d", what, object);  
        UF_CALL(UF_UI_write_listing_window(msg));  
        if (object == NULL_TAG)  
        {  
            UF_CALL(UF_UI_write_listing_window(" (NULL_TAG)\n"));  
            return;  
        }  
        status = UF_OBJ_ask_status(object);  
        sprintf(msg, ", status: %d", status);  
        UF_CALL(UF_UI_write_listing_window(msg));  
        if (status == UF_OBJ_DELETED)  
        {  
            UF_UI_write_listing_window("\n");  
            return;  
        }  
        if (!UF_OBJ_ask_name(object, name))  
        {  
            UF_UI_write_listing_window(", named: ");  
            UF_UI_write_listing_window(name);  
        }  
        UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
        ask_object_type_descriptor(type, subtype, msg);  
        UF_UI_write_listing_window(", type: ");  
        UF_UI_write_listing_window(msg);  
        if (UF_OBJ_is_object_a_promotion(object))  
            UF_UI_write_listing_window(", promotion");  
        if (UF_ASSEM_is_occurrence(object))  
        {  
            UF_CALL(UF_UI_write_listing_window(", occurrence"));  
            owner = UF_ASSEM_ask_part_occurrence(object);  
            if (owner != UF_ASSEM_ask_root_part_occ(part))  
            {  
                UF_CALL(UF_OBJ_ask_name(owner, msg));  
                UF_CALL(UF_UI_write_listing_window(" (from "));  
                UF_CALL(UF_UI_write_listing_window(msg));  
                UF_CALL(UF_UI_write_listing_window(")"));  
            }  
        }  
        UF_UI_write_listing_window("\n");  
    }  
    static void do_it(void)  
    {  
        unsigned int  
            ID,  
            version;  
        tag_t  
            object,  
            part = UF_PART_ask_display_part();  
        char  
            *file_data,  
            *handle,  
            msg[30];  
    /*  Note:  the "file_data" and "version" from the first object found in the  
     *  part will work to find any object in the part because the "version" is  
     *  not taken into consideration when finding the tag from the handle.  
     *  See PR 6650107.  
     */  
        object = UF_OBJ_cycle_all(part, NULL_TAG);  
        handle = UF_TAG_ask_handle_of_tag(object);  
        UF_CALL(UF_TAG_decompose_handle(handle, &file_data, &ID, &version));  
        UF_free(handle);  
        while (prompt_for_an_integer("Highlight/Name", "Object ID", &ID))  
        {  
            UF_CALL(UF_TAG_compose_handle(file_data, ID, version, &handle));  
            object = UF_TAG_ask_tag_of_handle(handle);  
            sprintf(msg, "ID%d", ID);  
            UF_CALL(UF_OBJ_set_name(object, msg));  
            RX(object);  
            if (object != NULL_TAG)  
            {  
                UF_CALL(UF_DISP_set_highlight(object, TRUE));  
                sprintf(msg, "Object ID %d highlighted", ID);  
                uc1601(msg, TRUE);  
                UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            }  
            UF_free(handle);  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
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

> 这段代码是一个NX Open C++ API的二次开发示例，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在API调用失败时打印错误信息。
> 2. 定义了一个提示用户输入整数的函数prompt_for_an_integer，用于获取用户输入。
> 3. 定义了一个查询对象类型描述符的函数ask_object_type_descriptor，用于根据对象类型获取其名称。
> 4. 定义了一个打印对象信息的函数report_simple_object_info，用于打印对象的基本信息。
> 5. 定义了主函数do_it，实现了以下功能：遍历当前部件中的所有对象，获取第一个对象的文件数据、ID和版本号。循环提示用户输入对象ID，根据ID查找对象并设置其名称。打印查找到的对象信息，高亮显示该对象，然后取消高亮。
> 6. 遍历当前部件中的所有对象，获取第一个对象的文件数据、ID和版本号。
> 7. 循环提示用户输入对象ID，根据ID查找对象并设置其名称。
> 8. 打印查找到的对象信息，高亮显示该对象，然后取消高亮。
> 9. 定义了ufusr函数，作为NX的二次开发入口函数，初始化API，调用do_it函数，然后终止API。
> 10. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 总体来说，这是一个典型的NX二次开发示例，实现了查询和操作部件中的对象的功能。代码结构清晰，具有较好的可读性。
>
