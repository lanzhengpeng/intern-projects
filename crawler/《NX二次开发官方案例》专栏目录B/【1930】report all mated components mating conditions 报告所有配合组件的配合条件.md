### 【1930】report all mated components mating conditions 报告所有配合组件的配合条件

#### 代码

```cpp
    /*HEAD REPORT_ALL_MATED_COMPONENTS_MATING_CONDITIONS CCC UFUN */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = %d\n", title, n);  
        ECHO(msg);  
    }  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
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
        strcpy(descriptor, "Unknown type");  /* In case its not found 里海译:如果找不到的话 */  
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
    #define WRITE_E(X) (write_expression_to_listing_window(#X, X))  
    void write_expression_to_listing_window(char *title, tag_t expression)  
    {  
        char  
            msg[MAX_LINE_SIZE+1],  
            *string;  
        if (expression == NULL_TAG)  
        {  
            sprintf(msg, "%s = NULL_TAG\n", title);  
            ECHO(msg);  
        }  
        else  
        {  
            UF_CALL(UF_MODL_ask_exp_tag_string(expression, &string));  
            sprintf(msg, "%s = %s\n", title, string);  
            ECHO(msg);  
            UF_free(string);  
        }  
    }  
    static void report_mc_load_status(char *name, UF_ASSEM_load_status_t status)  
    {  
        ECHO(name);  
        ECHO(" = ");  
        switch (status)  
        {  
            case UF_ASSEM_ok:  
                WRITE_D(UF_ASSEM_ok);  
                break;  
            case UF_ASSEM_suppressed:  
                WRITE_D(UF_ASSEM_suppressed);  
                break;  
            case UF_ASSEM_not_loaded:  
                WRITE_D(UF_ASSEM_not_loaded);  
                break;  
            case UF_ASSEM_instance_suppressed:  
                WRITE_D(UF_ASSEM_instance_suppressed);  
                break;  
            default:  
                ECHO("Unknown ");  
                WRITE_D(status);  
                break;  
        }  
    }  
    static void report_mc_mate_type(char *name, byte mate_type)  
    {  
        ECHO(name);  
        ECHO(" = ");  
        switch (mate_type)  
        {  
            case UF_ASSEM_mate:  
                WRITE_D(UF_ASSEM_mate);  
                break;  
            case UF_ASSEM_align:  
                WRITE_D(UF_ASSEM_align);  
                break;  
            case UF_ASSEM_orient:  
                WRITE_D(UF_ASSEM_orient);  
                break;  
            case UF_ASSEM_v16_mate:  
                WRITE_D(UF_ASSEM_v16_mate);  
                break;  
            case UF_ASSEM_v16_align:  
                WRITE_D(UF_ASSEM_v16_align);  
                break;  
            case UF_ASSEM_angle:  
                WRITE_D(UF_ASSEM_angle);  
                break;  
            case UF_ASSEM_distance:  
                WRITE_D(UF_ASSEM_distance);  
                break;  
            case UF_ASSEM_parallel:  
                WRITE_D(UF_ASSEM_parallel);  
                break;  
            case UF_ASSEM_perpendicular:  
                WRITE_D(UF_ASSEM_perpendicular);  
                break;  
            case UF_ASSEM_center:  
                WRITE_D(UF_ASSEM_center);  
                break;  
            case UF_ASSEM_tangent:  
                WRITE_D(UF_ASSEM_tangent);  
                break;  
            default:  
                ECHO("Unknown");  
                WRITE_D(mate_type);  
                break;  
        }  
    }  
    static void report_mc_from_or_to_type(char *name, byte type)  
    {  
        ECHO(name);  
        ECHO(" = ");  
        switch (type)  
        {  
            case UF_ASSEM_planar_face:  
                WRITE_D(UF_ASSEM_planar_face);  
                break;  
            case UF_ASSEM_cylindrical_face:  
                WRITE_D(UF_ASSEM_cylindrical_face);  
                break;  
            case UF_ASSEM_conical_face:  
                WRITE_D(UF_ASSEM_conical_face);  
                break;  
            case UF_ASSEM_toroidal_face:  
                WRITE_D(UF_ASSEM_toroidal_face);  
                break;  
            case UF_ASSEM_spherical_face:  
                WRITE_D(UF_ASSEM_spherical_face);  
                break;  
            case UF_ASSEM_axi_sym_face:  
                WRITE_D(UF_ASSEM_axi_sym_face);  
                break;  
            case UF_ASSEM_straight_edge:  
                WRITE_D(UF_ASSEM_straight_edge);  
                break;  
            case UF_ASSEM_datum_plane:  
                WRITE_D(UF_ASSEM_datum_plane);  
                break;  
            case UF_ASSEM_datum_axis:  
                WRITE_D(UF_ASSEM_datum_axis);  
                break;  
            case UF_ASSEM_csys:  
                WRITE_D(UF_ASSEM_csys);  
                break;  
            case UF_ASSEM_component:  
                WRITE_D(UF_ASSEM_component);  
                break;  
            case UF_ASSEM_point:  
                WRITE_D(UF_ASSEM_point);  
                break;  
            case UF_ASSEM_line:  
                WRITE_D(UF_ASSEM_line);  
                break;  
            case UF_ASSEM_circle_edge:  
                WRITE_D(UF_ASSEM_circle_edge);  
                break;  
            case UF_ASSEM_circle:  
                WRITE_D(UF_ASSEM_circle);  
                break;  
            case UF_ASSEM_spline:  
                WRITE_D(UF_ASSEM_spline);  
                break;  
            case UF_ASSEM_port:  
                WRITE_D(UF_ASSEM_port);  
                break;  
            case UF_ASSEM_parametric_face:  
                WRITE_D(UF_ASSEM_parametric_face);  
                break;  
            default:  
                ECHO("Unknown ");  
                WRITE_D(type);  
                break;  
        }  
    }  
    static void report_mc_sub_type(char *name, byte sub_type)  
    {  
        ECHO(name);  
        ECHO(" = ");  
        switch (sub_type)  
        {  
            case UF_ASSEM_planar_angle || UF_ASSEM_center_1_to_1:  
                ECHO("UF_ASSEM_planar_angle or ");  
                WRITE_D(UF_ASSEM_center_1_to_1);  
                break;  
            case UF_ASSEM_3d_angle || UF_ASSEM_center_1_to_2:  
                ECHO("UF_ASSEM_3d_angle or ");  
                WRITE_D(UF_ASSEM_center_1_to_2);  
                break;  
            case UF_ASSEM_center_2_to_1:  
                WRITE_D(UF_ASSEM_center_2_to_1);  
                break;  
            case UF_ASSEM_center_2_to_2:  
                WRITE_D(UF_ASSEM_center_2_to_2);  
                break;  
            default:  
                ECHO("Unknown ");  
                WRITE_D(sub_type);  
                break;  
        }  
    }  
    static void report_mc_help_points(char *name, byte has_help_point,  
        char *name2, double *help_points)  
    {  
        ECHO(name);  
        ECHO(" = ");  
        switch (has_help_point)  
        {  
            case UF_ASSEM_has_no_help_point:  
                WRITE_D(UF_ASSEM_has_no_help_point);  
                break;  
            case UF_ASSEM_has_help_point_on_from:  
                WRITE_D(UF_ASSEM_has_help_point_on_from);  
                write_double_array_to_listing_window(name2, help_points, 1, 3);  
                break;  
            case UF_ASSEM_has_help_point_on_to:  
                WRITE_D(UF_ASSEM_has_help_point_on_to);  
                write_double_array_to_listing_window(name2, help_points + 3, 1, 3);  
                break;  
            case UF_ASSEM_has_help_point_on_both:  
                WRITE_D(UF_ASSEM_has_help_point_on_both);  
                write_double_array_to_listing_window(name2, help_points, 1, 6);  
                break;  
            default:  
                ECHO("Unknown ");  
                WRITE_D(has_help_point);  
                break;  
        }  
    }  
    static void report_mating_conditions(UF_ASSEM_mating_condition_t *mc_data)  
    {  
        int  
            ii;  
        RX(mc_data->mated_object);  
        RX(mc_data->part_occurrence);  
        WRITE_S((char *)mc_data->name);  
        WRITE_L(mc_data->user_name);  
        for (ii = 0; ii < mc_data->num_constraints; ii++)  
        {  
            WRITE_D(ii);  
            report_mc_load_status("mc_data->constraints[ii].from_status",  
                mc_data->constraints[ii].from_status);  
            report_mc_load_status("mc_data->constraints[ii].to_status",  
                mc_data->constraints[ii].to_status);  
            report_mc_mate_type("mc_data->constraints[ii].mate_type",  
                mc_data->constraints[ii].mate_type);  
            report_mc_from_or_to_type("mc_data->constraints[ii].from_type",  
                mc_data->constraints[ii].from_type);  
            report_mc_from_or_to_type("mc_data->constraints[ii].to_type",  
                mc_data->constraints[ii].to_type);  
            RX(mc_data->constraints[ii].from);  
            RX(mc_data->constraints[ii].from_part_occ);  
            RX(mc_data->constraints[ii].to);  
            RX(mc_data->constraints[ii].to_part_occ);  
            WRITE_E(mc_data->constraints[ii].offset);  
            WRITE_S((char *)mc_data->constraints[ii].name);  
            WRITE_L(mc_data->constraints[ii].user_name);  
            WRITE_L(mc_data->constraints[ii].suppressed);  
            WRITE_L(mc_data->constraints[ii].inherited);  
            report_mc_sub_type("mc_data->constraints[ii].sub_type",  
                mc_data->constraints[ii].sub_type);  
            report_mc_from_or_to_type("mc_data->constraints[ii].second_from_type",  
                mc_data->constraints[ii].second_from_type);  
            RX(mc_data->constraints[ii].second_from);  
            RX(mc_data->constraints[ii].second_from_part_occ);  
            report_mc_from_or_to_type("mc_data->constraints[ii].second_to_type",  
                mc_data->constraints[ii].second_to_type);  
            RX(mc_data->constraints[ii].second_to);  
            RX(mc_data->constraints[ii].second_to_part_occ);  
            report_mc_help_points("mc_data->constraints[ii].has_help_point",  
                mc_data->constraints[ii].has_help_point,  
                "mc_data->constraints[ii].help_points",  
                mc_data->constraints[ii].help_points);  
        }  
        WRITE_D(mc_data->num_constraints);  
        WRITE_L(mc_data->suppressed);  
        WRITE_L(mc_data->override);  
        WRITE_L(mc_data->implicit);  
    }  
    static tag_t ask_next_component(tag_t part, tag_t comp)  
    {  
        int  
            subtype,  
            type;  
        while  (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_component_type, &comp))  
            && (comp != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(comp, &type, &subtype));  
            if (subtype == UF_part_occurrence_subtype) return comp;  
        }  
        return comp;  
    }  
    static void report_component_name(tag_t component_tag)  
    {  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_name[MAX_FSPEC_SIZE+1],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        WRITE_D(component_tag);  
        if (!UF_CALL(UF_ASSEM_ask_component_data(component_tag, part_name,  
            refset_name, instance_name, origin, csys_matrix, transform)))  
            WRITE_S(instance_name);  
    }  
    static void do_it(void)  
    {  
        logical  
            mated;  
        tag_t  
            comp = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_ASSEM_mating_condition_t  
            mc_data;  
        while ((comp = ask_next_component(part, comp)) != NULL_TAG)  
        {  
            report_component_name(comp);  
            UF_CALL(UF_ASSEM_ask_mc_data_of_compnt(comp, &mated, &mc_data));  
            if (mated) report_mating_conditions(&mc_data);  
            else { ECHO("Not mated.\n"); }  
            ECHO("\n");  
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
    static void check_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        for (ii=0; ii<status->n_parts; ii++)  
        {  
            UF_get_fail_message(status->statuses[ii], msg);  
            printf("    %s - %s\n", status->file_names[ii], msg);  
        }  
        UF_free(status->statuses);  
        UF_free_string_array(status->n_parts, status->file_names);  
    }  
    /*  To run in external native mode UG:  
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external UG/Manager or IMAN mode:  
            program.exe -pim=yes @DB/partno1/rev @DB/partno2/rev [...]  
    */  
    int main( int argc, char *argv[] )  
    {  
        int  
            cnt = 0;  
        tag_t  
            part;  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            while (uc4621(part_name) == 1)  
            {  
                printf("%d.  %s\n", ++cnt, part_name);  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                if (status.n_parts > 0) check_load_status(&status);  
                if (!status.failed)  
                {  
                    do_it();  
                    UF_CALL(UF_PART_close_all());  
                }  
            }  
            printf("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这是NX的二次开发代码，其主要功能是遍历装配体中的每个组件，并报告每个组件的配合条件信息。具体来说：
>
> 1. 初始化和循环：在UFusr函数中，首先进行NX的初始化，然后调用do_it函数。在do_it函数中，循环遍历装配体中的每个组件，并获取组件的名称。
> 2. 获取组件名称：使用ask_next_component函数获取下一个组件，并使用report_component_name函数报告组件的名称。
> 3. 获取配合条件：使用UF_ASSEM_ask_mc_data_of_compnt函数获取组件的配合条件数据，然后使用report_mating_conditions函数报告配合条件信息。
> 4. 报告配合条件信息：在report_mating_conditions函数中，报告每个配合条件的各种信息，如配合类型、参照对象、是否抑制等。
> 5. 释放内存：在main函数中，遍历传入的零件文件，打开每个零件，然后调用do_it函数，最后关闭所有零件文件。
> 6. 错误处理：代码中使用了宏定义来统一处理函数调用错误，并且提供了错误报告功能。
> 7. 代码注释：代码中包含详细的中文注释，有助于理解代码逻辑。
> 8. 代码规范：遵循了NX二次开发的代码规范，如函数命名、参数传递等。
>
> 总之，这段代码提供了遍历装配体组件并报告配合条件信息的二次开发功能，具有清晰的结构和良好的可读性。
>
