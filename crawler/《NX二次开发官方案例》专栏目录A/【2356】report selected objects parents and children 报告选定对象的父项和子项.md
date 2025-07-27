### 【2356】report selected objects parents and children 报告选定对象的父项和子项

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_OBJECTS_PARENTS_AND_CHILDREN CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_so.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 新增的功能。

翻译：

UF_print_syslog 是 V18 新增的功能。 */  
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
    /* qq3123197280 */  
    static int mask_add_stuff(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[3] = { { UF_section_segment_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_ENABLE_SPECIFIC, 3, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_anything(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_set_cursor_view(0));  
        UF_CALL(UF_UI_select_with_single_dialog( "Select an object", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_add_stuff, NULL, &resp,  
            &object, cp, &view));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
            case UF_circle_type: /* non-std format in the header file 里海译:头文件中的非标准格式翻译为：头文件中的非标准格式 */  
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
        strcpy(descriptor, "Unknown type");  /* In case its not found 里海译:如果找不到的话， */  
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
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        logical  
            is_so;  
        int  
            n_children,  
            n_parents;  
        tag_t  
            *children,  
            object,  
            *parents;  
        while ((object = select_anything("Report parents/children")) != NULL_TAG)  
        {  
            UF_CALL(UF_SO_is_subclass(object, &is_so));  
            WRITE_L(is_so);  
            if (!UF_CALL(UF_SO_ask_parents(object, UF_SO_ASK_ALL_PARENTS,  
                &n_parents, &parents)))  
            {  
                WRITE_D(n_parents);  
                WRITEnTt(n_parents, parents);  
                if (n_parents > 0) UF_free(parents);  
            }  
            if (!UF_CALL(UF_SO_ask_children(object, UF_SO_ASK_ALL_CHILDREN,  
                &n_children, &children)))  
            {  
                WRITE_D(n_children);  
                WRITEnTt(n_children, children);  
                if (n_children > 0) UF_free(children);  
            }  
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

> 根据代码内容，这是一个用于在NX中查询和报告对象（例如特征）的父对象和子对象的二次开发代码。下面是该代码的主要功能：
>
> 1. 定义了一个错误报告函数report_error，用于在出现UF函数调用错误时记录错误信息。
> 2. 定义了一个选择函数select_anything，用于让用户选择一个对象，并返回其tag。
> 3. 定义了一个描述对象类型的函数ask_object_type_descriptor，根据对象类型和子类型返回描述字符串。
> 4. 定义了几个宏，用于向列表窗口输出对象数组、布尔值和整数值。
> 5. 主函数do_it中，循环调用select_anything函数，获取用户选择的对象，然后使用UF_SO_is_subclass函数判断对象是否为特征子类，使用UF_SO_ask_parents和UF_SO_ask_children函数查询父对象和子对象，并输出到列表窗口。
> 6. ufusr函数是二次开发的入口函数，调用do_it函数，然后初始化和终止NX。
> 7. ufusr_ask_unload函数用于卸载二次开发。
>
> 总体来说，这是一个用于查询和报告NX对象关系的二次开发代码，包括查询父对象、子对象、对象类型等，并将结果输出到列表窗口。
>
