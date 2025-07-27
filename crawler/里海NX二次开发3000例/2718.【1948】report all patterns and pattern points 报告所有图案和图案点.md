### 【1948】report all patterns and pattern points 报告所有图案和图案点

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_exit.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_view.h>  
    #include <uf_obj.h>  
    #include <uf_so.h>  
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
    static tag_t ask_next_pattern(tag_t part, tag_t object)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_pattern_type, &object));  
        return (object);  
    }  
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
    static int ask_all_patterns(tag_t part, tag_t **patterns)  
    {  
        tag_t  
            pattern = NULL_TAG;  
        uf_list_p_t  
            patt_list;  
        UF_CALL(UF_MODL_create_list(&patt_list));  
        while ((pattern = ask_next_pattern(part, pattern)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(patt_list, pattern));  
        return (make_an_array(&patt_list, patterns));  
    }  
    #define RX(X) report_object_info(#X, X)  
    #define WRITEnT(n,X) { \  
    int ii; \  
    for (ii = 0; ii < n; ii++) \  
    RX(X[ii]); \  
    }  
    #define RXn(X,n)  WRITEnT(n,X)  
    static void ask_part_shortname(tag_t part, char *shortname)  
    {  
        logical  
            nxman;  
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_PART_ask_part_name(part, partname));  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        if (nxman)  
            UF_CALL(UF_PART_name_for_display(partname, shortname));  
        else  
            UF_CALL(uc4574(partname, 2, shortname));  
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
            case UF_circle_type: /* non-std format in the header file 里海译:在头文件中，non-std format 指的是非标准格式的声明或定义。 */  
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
                    if (strcmp(define, "gfem_load") && strcmp(define, "prefix1")) break;  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
    }  
    static void report_feature_name_type_and_sysname(tag_t feature)  
    {  
        char  
            *feature_name,  
            *feature_sysname,  
            *feature_type;  
        UF_CALL(UF_MODL_ask_feat_name( feature, &feature_name ));  
        WRITE_S(feature_name);  
        UF_free(feature_name);  
        UF_CALL(UF_MODL_ask_feat_sysname( feature, &feature_sysname ));  
        WRITE_S(feature_sysname);  
        UF_free(feature_sysname);  
        UF_CALL(UF_MODL_ask_feat_type( feature, &feature_type ));  
        WRITE_S(feature_type);  
        UF_free(feature_type);  
    }  
    static void report_object_info(char *what, tag_t object)  
    {  
        logical  
            is_displayable,  
            is_so;  
        int  
            status,  
            subtype,  
            type;  
        tag_t  
            owner,  
            part = UF_PART_ask_display_part();  
        char  
            name[UF_OBJ_NAME_LEN + 1],  
            owning_part[UF_CFI_MAX_FILE_NAME_SIZE],  
            msg[133];  
        UF_SO_visibility_option_t  
            vis_opt;  
        UF_OBJ_disp_props_t  
            disp_props;  
        ECHO("%s = %d", what, object);  
        if (object == NULL_TAG)  
        {  
            ECHO(" (NULL_TAG)\n\n");  
            return;  
        }  
        status = UF_OBJ_ask_status(object);  
        ECHO(", status: %d", status);  
        if (status == UF_OBJ_DELETED)  
        {  
            ECHO(" (Deleted)\n\n");  
            return;  
        }  
        UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
        ECHO(", type: %d, subtype: %d", type, subtype);  
        ask_object_type_descriptor(type, subtype, msg);  
        ECHO(" (%s)", msg);  
        if (type == UF_feature_type)  
        {  
            ECHO("\n");  
            report_feature_name_type_and_sysname(object);  
            ECHO("\n");  
            return;  
        }  
        if (UF_OBJ_is_object_a_promotion(object))  
        {  
            ECHO(", promotion");  
        }  
        if (UF_ASSEM_is_occurrence(object))  
        {  
            ECHO(", occurrence");  
            owner = UF_ASSEM_ask_part_occurrence(object);  
            if (owner != UF_ASSEM_ask_root_part_occ(part))  
            {  
                if (!UF_OBJ_ask_name(owner, msg))  
                    ECHO(" (from %s)", msg);  
                else  
                    ECHO(" (from root occ of)", msg);  
            }  
        }  
        if (!UF_CALL(UF_OBJ_ask_owning_part(object, &owner)))  
        {  
            ask_part_shortname(owner, owning_part);  
            ECHO(" owned by: %s", owning_part);  
        }  
        if (!uc6409(object, &status, name) && (status == 1))  
        {  
            ECHO(", view dependent: %s", name);  
        }  
        if (!UF_OBJ_ask_name(object, name))  
        {  
            ECHO(", name: %s", name);  
        }  
        UF_CALL(UF_SO_is_subclass(object, &is_so));  
        if (is_so)  
        {  
            UF_CALL(UF_SO_ask_visibility_option(object, &vis_opt));  
            if (vis_opt == UF_SO_invisible)  
            {  
                ECHO(", invisible SO");  
            }  
            else  
            {  
                ECHO(", visible SO");  
            }  
        }  
        UF_CALL(UF_OBJ_is_displayable(object, &is_displayable));  
        if (is_displayable)  
        {  
            UF_CALL(UF_OBJ_ask_display_properties(object, &disp_props));  
            ECHO("\n  layer: %d, color: %d, blank: %d, width: %d, "  
                "font: %d, highlight: %d\n\n", disp_props.layer,  
                disp_props.color, disp_props.blank_status,  
                disp_props.line_width, disp_props.font,  
                disp_props.highlight_status);  
        }  
        else  
            ECHO("\n\n");  
    }  
    static void do_it(void)  
    {  
        int  
            ii, n;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            *patterns;  
        n = ask_all_patterns(part, &patterns);  
        for (ii = 0; ii < n; ii++)  
            RX(patterns[ii]);  
    }  
    /* qq3123197280 */  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
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

> 这段代码是NX的二次开发代码，主要功能是遍历当前活动部件中的所有pattern，并打印出它们的详细信息。
>
> 具体来说，代码首先定义了错误处理函数ECHO，用于打印错误信息。然后定义了UF_CALL宏，用于调用NX API函数并处理错误。接着定义了ask_next_pattern函数，用于遍历部件中的pattern。
>
> ask_all_patterns函数用于获取所有pattern，并保存到一个数组中。report_object_info函数用于打印pattern的详细信息，包括类型、名称、拥有者等。do_it函数是主函数，首先获取当前活动部件，然后调用ask_all_patterns获取所有pattern，并遍历打印。
>
> 最后，定义了ufsta和ufusr两个函数，分别用于standalone和embedded模式下的运行。ufusr_ask_unload用于设置unload策略。
>
> 总体来说，这段代码是一个典型的NX二次开发示例，实现了遍历和打印pattern信息的功能。代码结构清晰，使用了一些典型的NX二次开发技巧，如错误处理、内存分配等。
>
