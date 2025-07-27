### 【2399】report udfs def data report_udfs_def_data函数报告UDF定义数据

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_so.h>  
    #include <uf_disp.h>  
    #include <uf_obj.h>  
    #include <uf_assem.h>  
    #include <uf_view.h>  
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
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
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
            case UF_circle_type: /* non-std format in the header file 里海译:非标准格式在头文件中 */  
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
        strcpy(descriptor, "Unknown type");  /* In case its not found 里海译:In case its not found, 如果找不到的话。 */  
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
    static void report_udfs_def_data(UF_MODL_udfs_def_data_t udfs_data)  
    {  
        int  
            ii,  
            jj;  
        WRITE_D(udfs_data.num_frecs);  
        RXn(udfs_dat.frecs, udfs_data.num_frecs);  
        WRITE_D(udfs_data.num_exp);  
        for (ii = 0; ii < udfs_data.num_exp; ii++)  
        {  
            WRITE_D(ii);  
            WRITE_D(udfs_data.exp_defs[ii]->type);  
            WRITE_S(udfs_data.exp_defs[ii]->exp);  
            WRITE_S(udfs_data.exp_defs[ii]->exp_name);  
            switch (udfs_data.exp_defs[ii]->type)  
            {  
                case UF_UDFS_EXP_OPTION:  
                    WRITE_D(udfs_data.exp_defs[ii]->num_def);  
                    for (jj = 0; jj < udfs_data.exp_defs[ii]->num_def; jj++)  
                    {  
                        WRITE_D(jj);  
                        WRITE_S(udfs_data.exp_defs[ii]->define_value[jj]);  
                    }  
                    break;  
                case UF_UDFS_EXP_RANGE:  
                    WRITE_S(udfs_data.exp_defs[ii]->low_end);  
                    WRITE_S(udfs_data.exp_defs[ii]->high_end);  
                    WRITE_D(udfs_data.exp_defs[ii]->scale_type);  
                    break;  
                default:  
                    break;  
            }  
        }  
        WRITE_D(udfs_data.num_ref);  
        for (ii = 0; ii < udfs_data.num_ref; ii++)  
        {  
            WRITE_D(ii);  
            RX(udfs_data.refs[ii]);  
            WRITE_S(udfs_data.ref_prompts[ii]);  
        }  
        WRITE_S(udfs_data.name_str_data);  
    }  
    static void free_udfs_def_data(UF_MODL_udfs_def_data_t udfs_data)  
    {  
        int  
            ii;  
        UF_free(udfs_data.frecs);  
        for (ii = 0; ii < udfs_data.num_exp; ii++)  
        {  
            UF_free(udfs_data.exp_defs[ii]->exp);  
            if (udfs_data.exp_defs[ii]->type == UF_UDFS_EXP_OPTION)  
                UF_free_string_array( udfs_data.exp_defs[ii]->num_def,  
                    udfs_data.exp_defs[ii]->define_value);  
        }  
        UF_free(udfs_data.exp_defs);  
        UF_free(udfs_data.refs);  
        UF_free_string_array(udfs_data.num_ref, udfs_data.ref_prompts);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            jj,  
            n_feats,  
            n_sets;  
        tag_t  
            feat,  
            *feats,  
            *sets;  
        UF_MODL_udfs_def_data_t  
            udfs_data;  
        char  
            *type;  
        while ((feat = select_a_feature("Select UDF")) != NULL_TAG)  
        {  
            RX(feat);  
            UF_CALL(UF_MODL_ask_feat_type(feat, &type));  
            if (!strcmp(type, "INSTNACE_SET")|| !strcmp(type, "INSTANCE_SET"))  
            {  
                UF_CALL(UF_MODL_ask_sets_of_member(feat, &sets, &n_sets));  
                for (ii = 0; ii < n_sets; ii++)  
                {  
                    UF_CALL(UF_MODL_ask_all_members_of_set(sets[ii], &feats,  
                        &n_feats));  
                    for (jj = 0; jj < n_feats; jj++)  
                    {  
                        UF_free(type);  
                        UF_CALL(UF_MODL_ask_feat_type(feats[jj], &type));  
                        if (!strcmp(type, "UDF_INS"))  
                        {  
                            feat = feats[jj];  
                            break;  
                        }  
                    }  
                    if (n_feats > 0) UF_free(feats);  
                    if (jj < n_feats) break;  
                }  
                if (n_sets > 0) UF_free(sets);  
            }  
            if (!strcmp(type, "UDF_INS"))  
            {  
                UF_CALL( UF_MODL_udfs_ask_def_data( feat, &udfs_data));  
                report_udfs_def_data(udfs_data);  
                free_udfs_def_data(udfs_data);  
            }  
            UF_free(type);  
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

> 这段代码是一个NX的二次开发示例，其主要功能是查询UDF特征的定义数据。下面是代码的主要逻辑和步骤：
>
> 1. 定义了错误报告函数report_error，用于在调用NX API失败时打印错误信息。
> 2. 定义了选择特征的函数select_a_feature，用于让用户选择一个特征。
> 3. 定义了查询特征名称、系统名称和类型的函数report_feature_name_type_and_sysname。
> 4. 定义了查询对象信息的函数report_object_info，用于打印出对象的各种属性，如名称、类型、拥有部件等。
> 5. 定义了查询UDF定义数据的函数report_udfs_def_data，用于打印出UDF的各种参数，如成员特征、表达式等。
> 6. 定义了释放UDF定义数据内存的函数free_udfs_def_data。
> 7. 定义了主函数do_it，其逻辑如下：循环让用户选择特征查询特征类型，如果是实例集，则查询其成员特征，直到找到UDF_INS类型的特征查询该UDF特征的定义数据，并打印释放定义数据内存
> 8. 循环让用户选择特征
> 9. 查询特征类型，如果是实例集，则查询其成员特征，直到找到UDF_INS类型的特征
> 10. 查询该UDF特征的定义数据，并打印
> 11. 释放定义数据内存
> 12. 定义了ufusr函数，作为NX的二次开发接口函数，用于初始化NX，调用主函数，然后终止NX。
> 13. 定义了卸载函数ufusr_ask_unload，表示立即卸载二次开发。
>
> 综上，这段代码的主要功能是查询UDF特征的定义数据，并以友好的方式打印出来，方便用户了解特征的具体信息。
>
