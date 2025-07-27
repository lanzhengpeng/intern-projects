### 【2347】report selected line arc feature data 报告所选线弧特征数据

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #include <uf_assem.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
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
    #define WRITE_L(X) if (X) ECHO("%s = TRUE\n", #X); \  
        else ECHO("%s = FALSE\n", #X);  
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
        strcpy(descriptor, "Unknown type");  /* In case its not found 里海译:如果找不到，请翻译为"In case its not found"。 */  
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
        ECHO("%s = %d", what, object);  
        if (object == NULL_TAG)  
        {  
            ECHO(" (NULL_TAG)\n");  
            return;  
        }  
        status = UF_OBJ_ask_status(object);  
        ECHO( ", status: %d", status);  
        if (status == UF_OBJ_DELETED)  
        {  
            ECHO("\n");  
            return;  
        }  
        if (!UF_OBJ_ask_name(object, name))  
        {  
            ECHO(", named: %s", name);  
        }  
        UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
        ask_object_type_descriptor(type, subtype, msg);  
        ECHO(", type: %s",msg);  
        if (UF_OBJ_is_object_a_promotion(object))  
            ECHO(", promotion");  
        if (UF_ASSEM_is_occurrence(object))  
        {  
            ECHO(", occurrence");  
            owner = UF_ASSEM_ask_part_occurrence(object);  
            if (owner != UF_ASSEM_ask_root_part_occ(part))  
            {  
                UF_CALL(UF_OBJ_ask_name(owner, msg));  
                ECHO(" (from %s)", msg);  
            }  
        }  
        ECHO("\n");  
    }  
    static void report_help_data(char *msg, UF_CURVE_help_data_t help_data)  
    {  
        ECHO(msg);  
        ECHO(".help_data_type = ");  
        switch (help_data.help_data_type)  
        {  
            case UF_CURVE_help_data_none:  
                ECHO("UF_CURVE_help_data_none;\n");  
                break;  
            case UF_CURVE_help_data_parameter:  
                ECHO("UF_CURVE_help_data_parameter;\n");  
                ECHO(msg);  
                ECHO(".parameter = %f;\n", help_data.parameter);  
                break;  
            case UF_CURVE_help_data_value:  
                ECHO("UF_CURVE_help_data_value;\n");  
                ECHO(msg);  
                ECHO(".value[0] = %f;\n", help_data.value[0]);  
                ECHO(msg);  
                ECHO(".value[1] = %f;\n", help_data.value[1]);  
                ECHO(msg);  
                ECHO(".value[2] = %f;\n", help_data.value[2]);  
                break;  
            default:  
                ECHO("%d (undefined)\n", help_data.help_data_type);  
                break;  
        }  
    }  
    static void report_curve_constraint(char *msg, UF_CURVE_constraint_t constraint)  
    {  
        char  
            help_msg[MAX_LINE_SIZE+1];  
        ECHO(msg);  
        ECHO(".constraint_type = ");  
        switch (constraint.constraint_type)  
        {  
            case UF_CURVE_constraint_none:  
                ECHO("UF_CURVE_constraint_none;\n");  
                break;  
            case UF_CURVE_coincident:  
                ECHO("UF_CURVE_coincident;\n");  
                break;  
            case UF_CURVE_tangent:  
                ECHO("UF_CURVE_tangent;\n");  
                break;  
            case UF_CURVE_normal:  
                ECHO("UF_CURVE_normal;\n");  
                break;  
            case UF_CURVE_angle:  
                ECHO("UF_CURVE_angle;\n");  
                break;  
            case UF_CURVE_along_x:  
                ECHO("UF_CURVE_along_x;\n");  
                break;  
            case UF_CURVE_along_y:  
                ECHO("UF_CURVE_along_y;\n");  
                break;  
            case UF_CURVE_along_z:  
                ECHO("UF_CURVE_along_z;\n");  
                break;  
            case UF_CURVE_radius:  
                ECHO("UF_CURVE_radius;\n");  
                break;  
            default:  
                ECHO("%d (undefined)\n", constraint.constraint_type);  
                break;  
        }  
        ECHO(msg);  
        ECHO(".end_type = ");  
        switch (constraint.end_type)  
        {  
            case UF_CURVE_start:  
                ECHO("UF_CURVE_start;\n");  
                break;  
            case UF_CURVE_end:  
                ECHO("UF_CURVE_end;\n");  
                break;  
            case UF_CURVE_middle:  
                ECHO("UF_CURVE_middle;\n");  
                break;  
            case UF_CURVE_center:  
                ECHO("UF_CURVE_center;\n");  
                break;  
            default:  
                ECHO("%d (undefined)\n", constraint.end_type);  
                break;  
        }  
        ECHO(msg);  
        ECHO(".object_tag = ");  
        RX(constraint.object_tag);  
        ECHO(msg);  
        ECHO(".value = %f;\n", constraint.value);  
        if ((constraint.constraint_type == UF_CURVE_tangent) ||  
            (constraint.constraint_type == UF_CURVE_normal))  
        {  
            sprintf(help_msg, "%s.help_data", msg);  
            report_help_data(help_msg, constraint.help_data);  
        }  
    }  
    static void report_curve_limit(char *msg, UF_CURVE_limit_t limits)  
    {  
        char  
            help_msg[MAX_LINE_SIZE+1];  
        ECHO(msg);  
        ECHO(".limit_type = ");  
        switch (limits.limit_type)  
        {  
            case UF_CURVE_limit_to_constraint:  
                ECHO("UF_CURVE_limit_to_constraint;\n");  
                break;  
            case UF_CURVE_limit_value:  
                ECHO("UF_CURVE_limit_value;\n");  
                break;  
            case UF_CURVE_limit_to_entity:  
                ECHO("UF_CURVE_limit_to_entity;\n");  
                break;  
            default: ECHO("%d (undefined)\n", limits.limit_type);  
                break;  
        }  
        if (limits.limit_type == UF_CURVE_limit_value)  
        {  
            ECHO(msg);  
            ECHO(".value = %f;\n", limits.value);  
        }  
        if (limits.limit_type == UF_CURVE_limit_to_entity)  
        {  
            ECHO(msg);  
            ECHO(".limiting_obj = ");  
            RX(limits.limiting_obj);  
        }  
        sprintf(help_msg, "%s.help_data", msg);  
        report_help_data(help_msg, limits.help_data);  
    }  
    static void report_line_arc_data(tag_t feat)  
    {  
        int  
            ii;  
        char  
            msg[MAX_LINE_SIZE];  
        UF_CURVE_line_arc_t  
            data;  
        if (UF_CALL(UF_CURVE_ask_line_arc_data(feat, &data))) return;  
        ECHO("data.curve_type = ");  
        switch (data.curve_type)  
        {  
            case UF_CURVE_asso_none:  
                ECHO("UF_CURVE_asso_none;\n");  
                break;  
            case UF_CURVE_asso_line:  
                ECHO("UF_CURVE_asso_line;\n");  
                break;  
            case UF_CURVE_asso_arc:  
                ECHO("UF_CURVE_asso_arc;\n");  
                break;  
            default:  
                ECHO("%d (undefined)\n", data.curve_type);  
                break;  
        }  
        ECHO("data.arc_constraint_subtype = ");  
        switch (data.arc_constraint_subtype)  
        {  
            case UF_CURVE_line_arc_three_point_arc:  
                ECHO("UF_CURVE_line_arc_three_point_arc;\n");  
                break;  
            case UF_CURVE_asso_arc_from_center:  
                ECHO("UF_CURVE_asso_arc_from_center;\n");  
                break;  
            default:  
                ECHO("%d (undefined)\n", data.arc_constraint_subtype);  
                break;  
        }  
        for (ii = 0; ii < 3; ii++)  
        {  
            if ((ii < 2) || ((data.curve_type == UF_CURVE_asso_arc) &&  
                (data.arc_constraint_subtype == UF_CURVE_line_arc_three_point_arc)))  
            {  
                sprintf(msg, "data.constraints[%d]", ii);  
                report_curve_constraint(msg, data.constraints[ii]);  
            }  
        }  
        for (ii = 0; ii < 2; ii++)  
        {  
            sprintf(msg, "data.limits[%d]", ii);  
            report_curve_limit(msg, data.limits[ii]);  
        }  
        RX(data.plane_of_curve);  
        WRITE_L(data.complement);  
        WRITE_L(data.closed);  
        WRITE_L(data.is_associative);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat;  
        while ((feat = select_a_feature("Select Line/Arc")) != NULL_TAG)  
            report_line_arc_data(feat);  
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

> 这段代码是一个NX的二次开发示例，主要功能是查询并报告选中线/弧特征的相关信息。下面是代码的主要功能和流程：
>
> 1. 包含了NX二次开发常用的头文件。
> 2. 定义了ECHO函数，用于打印信息到列表窗口和系统日志。
> 3. 定义了UF_CALL宏，用于调用NX API函数，并捕获错误。
> 4. 定义了RX宏，用于报告一个对象的信息。
> 5. 定义了WRITE_L宏，用于打印布尔值。
> 6. 定义了ask_object_type_descriptor函数，用于根据对象类型获取描述字符串。
> 7. 定义了report_simple_object_info函数，用于报告一个对象的基本信息。
> 8. 定义了report_help_data函数，用于报告曲线帮助数据。
> 9. 定义了report_curve_constraint函数，用于报告曲线约束信息。
> 10. 定义了report_curve_limit函数，用于报告曲线限制信息。
> 11. 定义了report_line_arc_data函数，用于报告线/弧特征的数据。
> 12. 定义了do_it函数，用于循环选择线/弧特征，并报告其信息。
> 13. 定义了ufusr函数，为NX二次开发的入口函数。在该函数中初始化NX系统，调用do_it函数，然后终止NX系统。
> 14. 定义了ufusr_ask_unload函数，用于卸载二次开发程序。
>
> 总的来说，这段代码提供了一个简单的NX二次开发示例，实现了查询并打印线/弧特征信息的功能。
>
