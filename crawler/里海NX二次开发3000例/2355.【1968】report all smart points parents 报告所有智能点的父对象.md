### 【1968】report all smart points parents 报告所有智能点的父对象

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_assem.h>  
    #include <uf_view.h>  
    #include <uf_object_types.h>  
    #include <uf_so.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
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
    static tag_t ask_next_smart_point(tag_t part, tag_t point)  
    {  
        logical  
            is_so;  
        int  
            subtype,  
            type;  
        /*  
            Note: UF_OBJ_cycle_objs_in_part will not find invisible smart  
            points so UF_OBJ_cycle_all is used here instead  
        */  
        while ((point = UF_OBJ_cycle_all(part, point)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(point, &type, &subtype));  
            if (type == UF_point_type)  
            {  
                UF_CALL(UF_SO_is_subclass(point, &is_so));  
                if (is_so) return point;  
            }  
        }  
        return NULL_TAG;  
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
        strcpy(descriptor, "Unknown type");  /* In case its not found 里海译:如果找不到的话。 */  
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
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    static void report_smart_objects_parents(tag_t so)  
    {  
        int  
            n_parents;  
        tag_t  
            *parents;  
        if (UF_CALL(UF_SO_ask_parents(so, UF_SO_ASK_ALL_PARENTS, &n_parents,  
            &parents))) return ;  
        WRITE_D(n_parents);  
        RXn(parents, n_parents);  
        if (n_parents > 0) UF_free(parents);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part(),  
            so = NULL_TAG;  
        while ((so = ask_next_smart_point(part, so)) != NULL_TAG)  
        {  
            RX(so);  
            report_smart_objects_parents(so);  
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

> 这段代码是用于NX的二次开发，主要功能是遍历当前显示部件中的所有智能点(Smart Object)并打印相关信息。以下是代码的主要内容和逻辑：
>
> 1. 头文件包含：代码包含了NX二次开发常用的头文件，如uf.h、uf_ui.h、uf_obj.h等。
> 2. 宏定义：定义了一些宏，如ECHO用于打印信息，UF_CALL用于调用NX API并处理错误，RX用于打印对象信息等。
> 3. 错误处理函数：定义了report_error函数，用于处理NX API调用错误。
> 4. 遍历智能点函数：ask_next_smart_point函数用于遍历当前部件中的所有智能点，并返回下一个智能点。
> 5. 打印对象信息函数：report_object_info函数用于打印对象的各种信息，如类型、子类型、名称、拥有者等。
> 6. 打印智能点父对象函数：report_smart_objects_parents函数用于打印智能点的所有父对象。
> 7. 主体函数：do_it函数用于遍历当前显示部件中的所有智能点，并打印相关信息。
> 8. API初始化和终止：ufusr函数是NX二次开发的入口函数，用于初始化NX API并调用do_it函数，最后终止NX API。
> 9. 卸载函数：ufusr_ask_unload函数用于处理二次开发的卸载。
>
> 总体来说，这段代码实现了对当前显示部件中的所有智能点进行遍历和打印相关信息的功能。
>
