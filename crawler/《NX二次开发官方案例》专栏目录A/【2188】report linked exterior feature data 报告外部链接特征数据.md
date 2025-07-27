### 【2188】report linked exterior feature data 报告外部链接特征数据

#### 代码

```cpp
    #define WRITE(X)   ECHO(X)  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        ECHO(msg);  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = %d\n", title, n);  
        ECHO(msg);  
    }  
    #define WRITEnD(n,X) (write_integer_array_to_listing_window(#X, X, 1, n))  
    static void write_integer_array_to_listing_window(char *name, int *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) ECHO(msg);  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%d", array[kk++]);  
                ECHO(msg);  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    ECHO(msg);  
                }  
            }  
            if (cols > 0) ECHO("\n");  
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
            case UF_circle_type: /* non-std format in the header file 译:在头文件中定义非标准格式 */  
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
        strcpy(descriptor, "Unknown type");  /* In case its not found 译:如果找不到的话 */  
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
        UF_CALL(UF_OBJ_is_displayable(object, &is_displayable));  
        if (is_displayable)  
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
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = %f\n", title, n);  
        ECHO(msg);  
    }  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) ECHO(msg);  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                ECHO(msg);  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    ECHO(msg);  
                }  
            }  
            if (cols > 0) ECHO("\n");  
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

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，主要功能是查询和分析NX模型中外部链接特征(Linked External Feature)的数据，包括外部链接的体、面信息、质量属性等。以下是主要功能的简要介绍：
>
> 1. 定义了多个宏，用于格式化输出不同类型的数据。
> 2. 定义了report_object_info函数，用于查询和分析NX对象的信息，包括类型、名称、拥有部件、显示属性等。
> 3. 定义了report_mass_properties函数，用于输出模型的质量属性，如表面积、体积、质量、质心、转动惯量等。
> 4. 定义了report_linked_exterior_data函数，用于输出外部链接特征的数据，如链接的体、面信息，分组方式等。
> 5. 定义了free_linked_exterior_data函数，用于释放外部链接特征数据结构中的动态内存。
> 6. 定义了report_linked_exterior_feature_data函数，用于查询和分析NX模型中指定外部链接特征的数据，并调用上述函数输出。
> 7. 该代码通过NX Open C++ API查询和分析NX模型数据，具有参数化、可复用的特点。
>
