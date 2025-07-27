### 【2349】report selected linked exterior feature data 报告选定的关联外部特征数据

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_view.h>  
    #include <uf_vec.h>  
    #include <uf_part.h>  
    #include <uf_undo.h>  
    #include <uf_disp.h>  
    #include <uf_so.h>  
    #include <uf_assem.h>  
    #include <uf_group.h>  
    #include <uf_linked_exterior.h>  
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
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
    }  
    #define WRITE(X)   ECHO(X)  
    #define WRITE_L(X) if (X) ECHO("%s = TRUE\n", #X); \  
        else ECHO("%s = FALSE\n", #X);  
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    #define WRITEnD(n,X) (write_integer_array_to_listing_window(#X, X, 1, n))  
    static void write_integer_array_to_listing_window(char *name, int *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (cols > 0)  
                if (ii == 0) ECHO("%s = ", name);  
                else ECHO("%s", blanks);  
            for (jj = 0; jj < cols; jj++)  
            {  
                ECHO("%d", array[kk++]);  
                if (jj < cols - 1)  
                {  
                    ECHO("%s", ", ");  
                }  
            }  
            if (cols > 0) ECHO("%s", "\n");  
        }  
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
        char  
            partname[MAX_FSPEC_SIZE+1];  
        UF_CALL(UF_PART_ask_part_name(part, partname));  
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
    static void report_object_info(char *what, tag_t object)  
    {  
        logical  
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
        sprintf(msg, "%s = %d", what, object);  
        ECHO(msg);  
        if (object == NULL_TAG)  
        {  
            ECHO(" (NULL_TAG)\n\n");  
            return;  
        }  
        status = UF_OBJ_ask_status(object);  
        sprintf(msg, ", status: %d", status);  
        ECHO(msg);  
        if (status == UF_OBJ_DELETED)  
        {  
            ECHO(" (Deleted)\n\n");  
            return;  
        }  
        UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
        sprintf(msg, ", type: %d, subtype: %d", type, subtype);  
        ECHO(msg);  
        ask_object_type_descriptor(type, subtype, msg);  
        ECHO(" (");  
        ECHO(msg);  
        ECHO(")");  
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
        if (!UF_CALL(UF_OBJ_ask_owning_part(object, &owner)) &&  
            (owner != part))  
        {  
            ask_part_shortname(owner, owning_part);  
            sprintf(msg, ", owned by: %s", owning_part);  
            ECHO(msg);  
        }  
        if (!uc6409(object, &status, name) && (status == 1))  
        {  
            sprintf(msg, ", view dependent: %s", name);  
            ECHO(msg);  
        }  
        if (!UF_OBJ_ask_name(object, name))  
        {  
            sprintf(msg, ", name: %s", name);  
            ECHO(msg);  
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
        if (UF_OBJ_is_type_displayable(type))  
        {  
            UF_CALL(UF_OBJ_ask_display_properties(object, &disp_props));  
            sprintf(msg, "\n  layer: %d, color: %d, blank: %d, width: %d, "  
                "font: %d, highlight: %d\n\n", disp_props.layer,  
                disp_props.color, disp_props.blank_status,  
                disp_props.line_width, disp_props.font,  
                disp_props.highlight_status);  
            ECHO(msg);  
        }  
        else  
            ECHO("\n\n");  
    }  
    #define WRITE_F(X) ECHO("%s = %f\n", #X, X)  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1];  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (cols > 0)  
                if (ii == 0) ECHO("%s = ", name);  
                else ECHO("%s", blanks);  
            for (jj = 0; jj < cols; jj++)  
            {  
                ECHO("%f", array[kk++]);  
                if (jj < cols - 1) ECHO("%s", ", ");  
            }  
            if (cols > 0) ECHO("%s", "\n");  
        }  
    }  
    static void report_mass_properties(double *mass_props)  
    {  
        write_double_to_listing_window("Surface Area", mass_props[0]);  
        write_double_to_listing_window("Volume", mass_props[1]);  
        write_double_to_listing_window("Mass", mass_props[2]);  
        write_double_array_to_listing_window("Center of Mass, WCS",  
            &mass_props[3], 1, 3);  
        write_double_array_to_listing_window("First Moments, Centroidal",  
            &mass_props[6], 1, 3);  
        write_double_array_to_listing_window("Moments of Inertia, WCS",  
            &mass_props[9], 1, 3);  
        write_double_array_to_listing_window("Moments of Inertia, Centroidal",  
            &mass_props[12], 1, 3);  
        write_double_to_listing_window("Spherical Moment of inertia",  
            mass_props[15]);  
        write_double_array_to_listing_window("Inertia Products, WCS",  
            &mass_props[16], 1, 3);  
        write_double_array_to_listing_window("Inertia Products, Centroidal",  
            &mass_props[19], 1, 3);  
        write_double_array_to_listing_window("Principal Axes, WCS",  
            &mass_props[22], 3, 3);  
        write_double_array_to_listing_window("Principal Axes, Centroidal",  
            &mass_props[31], 3, 3);  
        write_double_array_to_listing_window("Radius of Gyration, WCS",  
            &mass_props[34], 1, 3);  
        write_double_array_to_listing_window("Radius of Gyration, Centroidal",  
            &mass_props[37], 1, 3);  
        write_double_to_listing_window("Spherical Radius of Gyration",  
            mass_props[40]);  
        write_double_to_listing_window("Density", mass_props[46]);  
    }  
    static void report_linked_exterior_data(UF_MODL_linked_ext_p_t ext_data)  
    {  
        WRITE_D(ext_data->num_bodies);  
        WRITEnT(ext_data->num_bodies, ext_data->bodies);  
        WRITEnT(ext_data->num_bodies, ext_data->xforms);  
        WRITE_D(ext_data->num_faces);  
        WRITEnT(ext_data->num_faces, ext_data->faces);  
        WRITEnD(ext_data->num_faces, ext_data->xform_index);  
        switch (ext_data->group_results)  
        {  
            case UF_LINKED_EXT_GROUP_NONE:  
                WRITE("UF_LINKED_EXT_GROUP_NONE ");  
                break;  
            case UF_LINKED_EXT_GROUP_SINGLE:  
                WRITE("UF_LINKED_EXT_GROUP_SINGLE ");  
                break;  
            case UF_LINKED_EXT_GROUP_BY_COMPONENT:  
                WRITE("UF_LINKED_EXT_GROUP_BY_COMPONENT ");  
            default:  
                break;  
        }  
        WRITE_D(ext_data->group_results);  
        WRITE_L(ext_data->mass_props);  
        WRITE_L(ext_data->delete_openings);  
        WRITE_L(ext_data->at_timestamp);  
    }  
    static void free_linked_exterior_data(UF_MODL_linked_ext_p_t ext_data)  
    {  
        if (ext_data->num_bodies > 0)  
        {  
            UF_free(ext_data->bodies);  
            UF_free(ext_data->xforms);  
        }  
        if (ext_data->num_faces > 0)  
        {  
            UF_free(ext_data->faces);  
            UF_free(ext_data->xform_index);  
        }  
        ext_data->num_bodies = 0;  
        ext_data->bodies = NULL;  
        ext_data->xforms = NULL;  
        ext_data->num_faces = 0;  
        ext_data->faces = NULL;  
        ext_data->xform_index = NULL;  
    }  
    static void report_linked_exterior_feature_data(tag_t ext_feat)  
    {  
        int  
            n_groups = 0,  
            n_subfeats = 0;  
        tag_t  
            *groups = NULL,  
            *subfeats = NULL;  
        double  
            mass_props[47] = {0};  
        UF_MODL_linked_ext_t  
            ext_data = {0};  
        if (UF_CALL(UF_MODL_ask_linked_exterior(ext_feat, &ext_data,  
            &n_groups, &groups, &n_subfeats, &subfeats, mass_props))) return;  
        report_linked_exterior_data(&ext_data);  
        free_linked_exterior_data(&ext_data);  
        WRITE_D(n_groups);  
        WRITEnT(n_groups, groups);  
        if (n_groups > 0) UF_free(groups);  
        WRITE_D(n_subfeats);  
        WRITEnT(n_subfeats, subfeats);  
        if (n_subfeats > 0) UF_free(subfeats);  
        report_mass_properties(mass_props);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat;  
        while ((feat = select_a_feature("Select Linked Exterior")) != NULL_TAG)  
            report_linked_exterior_feature_data(feat);  
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

> 这段代码是用于NX的二次开发，主要功能是查询和报告NX模型中链接外表面(linked exterior)特征的信息。
>
> 关键点如下：
>
> 1. 定义了一些宏，用于格式化打印日志信息。
> 2. 定义了函数select_a_feature，用于让用户选择一个链接外表面特征。
> 3. 定义了函数report_linked_exterior_data，用于报告链接外表面特征的数据，包括体、面、变换矩阵等信息。
> 4. 定义了函数report_mass_properties，用于报告质量属性信息，如体积、质量、质心等。
> 5. 主函数do_it会循环调用select_a_feature和report_linked_exterior_data，让用户选择链接外表面特征并报告其信息。
> 6. ufusr是NX的回调函数，在NX启动时调用，会初始化NX环境并执行do_it函数。
> 7. ufusr_ask_unload函数用于处理插件卸载。
> 8. 代码中使用了NX的UF模块，包括UF_OBJ、UF_MODL、UF_ASSEM等，用于查询对象、模型和装配信息。
> 9. 使用了标准的错误处理机制，能够捕捉并打印错误信息。
>
> 总体来说，这段代码通过UF API实现了查询链接外表面特征信息的功能，使用了NX提供的二次开发接口，实现了交互式的信息查询。
>
