### 【1975】report all tabular note objects 报告所有表格注释对象

#### 代码

```cpp
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static int ask_all_of_type(tag_t part, int type, tag_t **objects)  
    {  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object)) &&  
                (object != NULL_TAG))  
            UF_CALL(UF_MODL_put_list_item(object_list, object));  
        return (make_an_array(&object_list, objects));  
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
            case UF_circle_type: /* non-std format in the header file 译:非标准格式在头文件中 */  
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
        strcpy(descriptor, "Unknown type");  /* In case its not found 译:In case its not found，如果没有找到 */  
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
    #define WRITEnTt(n,X) (report_object_array_types(n, #X, X))  
    static void report_object_array_types(int n_objects, char *title, tag_t *objects)  
    {  
        int  
            ii,  
            subtype,  
            type;  
        char  
            msg[UF_UI_MAX_STRING_LEN+1],  
            type_desc[MAX_LINE_SIZE+1];  
        if (n_objects < 1) return;  
        UF_CALL(UF_UI_open_listing_window());  
        for (ii = 0; ii < n_objects; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(objects[ii], &type, &subtype));  
            ask_object_type_descriptor(type, subtype, type_desc);  
            sprintf(msg, "%s[%d] = %d, %s\n", title, ii, objects[ii], type_desc);  
            UF_CALL(UF_UI_write_listing_window(msg));  
        }  
    }  
    static void report_all_tabular_note_objects(void)  
    {  
        int  
            n_objects;  
        tag_t  
            *objects,  
            part = UF_PART_ask_display_part();  
        n_objects = ask_all_of_type(part, UF_tabular_note_type, &objects);  
        WRITEnTt(n_objects, objects);  
    }

```

#### 代码解析

> 这段NX二次开发代码主要用于在NX中查询并报告所有表格注释对象的信息。
>
> 主要功能包括：
>
> 1. 查询表格注释对象：通过UF_MODL_ask_list_count、UF_OBJ_cycle_objs_in_part等函数，查询当前显示部件中的所有表格注释对象，并将它们的id存储在数组中。
> 2. 获取对象类型描述：通过自定义的ask_object_type_descriptor函数，根据对象类型和子类型，从头文件中解析出对象类型描述字符串。
> 3. 报告对象信息：通过自定义的report_object_array_types函数，打开NX的输出窗口，并按照特定格式打印出每个表格注释对象的id和类型描述。
> 4. 释放内存：在查询完成后，释放存储对象id数组的内存。
> 5. 主函数：report_all_tabular_note_objects函数为程序的入口，负责查询当前显示部件中的所有表格注释对象，并报告这些对象的信息。
>
> 通过这段代码，用户可以快速查询并了解当前NX部件中的所有表格注释对象，为后续的二次开发提供便利。
>
