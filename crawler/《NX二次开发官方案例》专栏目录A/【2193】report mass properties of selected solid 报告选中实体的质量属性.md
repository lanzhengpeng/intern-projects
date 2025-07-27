### 【2193】report mass properties of selected solid 报告选中实体的质量属性

#### 代码

```cpp
    /*HEAD REPORT_MASS_PROPERTIES_OF_SELECTED_SOLID CCC UFUN */  
    #include <stdarg.h>  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_BUFSIZE];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), _TRUNCATE, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error(char *file, int line, char *call, int irc)  
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
    /* qq3123197280 */  
    static int filter_body_type(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        int  
            body_type;  
        UF_CALL(UF_MODL_ask_body_type(object, &body_type));  
        if (body_type == *(int *)data)  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    static int mask_body_type(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_body_type, NULL, type)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_solid(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            solid = UF_MODL_SOLID_BODY,  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a solid", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_body_type, &solid, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void write_double_array_to_listing_window(char *name, double *array,  
        int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN + 1],  
            msg[UF_UI_MAX_STRING_LEN + 1];  
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
    static void report_mass_properties(tag_t solid)  
    {  
        int  
            units;  
        tag_t  
            part = UF_PART_ask_display_part();  
        double  
            acc[11] = { 0.99, 0,0,0,0,0,0,0,0,0,0 },  
            mass_props[47],  
            stats[13];  
        UF_CALL(UF_PART_ask_units(part, &units));  
        if (units == UF_PART_METRIC)  
            units = UF_MODL_grams_centimeters;  
        else  
            units = UF_MODL_pounds_inches;  
        if (UF_CALL(UF_MODL_ask_mass_props_3d(&solid, 1, 1, units, 1, 1, acc,  
            mass_props, stats))) return;  
        ECHO("Surface Area = %f\n", mass_props[0]);  
        ECHO("Surface Area error = %f\n", stats[0]);  
        ECHO("Volume = %f\n", mass_props[1]);  
        ECHO("Volume error = %f\n", stats[1]);  
        ECHO("Mass = %f\n", mass_props[2]);  
        ECHO("Mass error = %f\n", stats[2]);  
        write_double_array_to_listing_window("Center of Mass, WCS", &mass_props[3], 1, 3);  
        ECHO("Center of Mass error sphere radius = %f\n", stats[3]);  
        write_double_array_to_listing_window("First Moments, Centroidal", &mass_props[6], 1, 3);  
        write_double_array_to_listing_window("Moments errors", &stats[4], 1, 3);  
        write_double_array_to_listing_window("Moments of Inertia, WCS", &mass_props[9], 1, 3);  
        write_double_array_to_listing_window("Moments of Inertia, WCS errors", &stats[7], 1, 3);  
        write_double_array_to_listing_window("Moments of Inertia, Centroidal", &mass_props[12], 1, 3);  
        ECHO("Spherical Moment of inertia = %f\n", mass_props[15]);  
        write_double_array_to_listing_window("Inertia Products, WCS", &mass_props[16], 1, 3);  
        write_double_array_to_listing_window("Inertia Products, WCS errors", &stats[10], 1, 3);  
        write_double_array_to_listing_window("Inertia Products, Centroidal", &mass_props[19], 1, 3);  
        write_double_array_to_listing_window("Principal Axes, WCS", &mass_props[22], 3, 3);  
        write_double_array_to_listing_window("Principal Axes, Centroidal", &mass_props[31], 3, 3);  
        write_double_array_to_listing_window("Radius of Gyration, WCS", &mass_props[34], 1, 3);  
        write_double_array_to_listing_window("Radius of Gyration, Centroidal", &mass_props[37], 1, 3);  
        ECHO("Spherical Radius of Gyration = %f\n", mass_props[40]);  
        ECHO("Density = %f\n", mass_props[46]);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            solid;  
        while ((solid = select_a_solid("Report Mass Properties")) != NULL_TAG)  
            report_mass_properties(solid);  
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

> 根据代码，这是一个NX二次开发程序，主要实现了以下功能：
>
> 1. 选择实体：通过UF_UI_select_with_single_dialog函数让用户选择一个实体，并使用自定义的过滤函数确保只选择实体类型。
> 2. 计算并报告质量属性：使用UF_MODL_ask_mass_props_3d函数计算所选实体的质量属性，如表面积、体积、质量、质心等，并将这些属性输出到列表窗口。
> 3. 错误处理：定义了一个错误处理函数report_error，用于在出错时输出错误码和错误信息。
> 4. 代码结构：主函数do_it中循环调用选择实体和报告质量属性的函数，直到用户选择退出。
> 5. 标准初始化和终止：在ufusr函数中进行标准的初始化和终止操作。
> 6. 立即卸载：定义了ufusr_ask_unload函数，以确保立即卸载该程序。
>
> 综上所述，该程序实现了在NX中让用户选择实体并计算并报告其质量属性的功能，具有完善的错误处理和用户交互。
>
