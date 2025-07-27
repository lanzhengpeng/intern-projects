### 【2194】report mass properties of selected solids 报告选定实体的质量特性

#### 代码

```cpp
    /*HEAD REPORT_MASS_PROPERTIES_OF_SELECTED_SOLIDS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据提供的信息，可以翻译为：

注意：UF_print_syslog是V18版本新增的。 */  
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
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_bodies(char *prompt, tag_t **bodies)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select solids or sheets", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp, &cnt,  
            bodies));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*bodies)[ii], FALSE));  
        return cnt;  
    }  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    static void report_mass_properties(int num_solids, tag_t *solids)  
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
        if (UF_CALL(UF_MODL_ask_mass_props_3d(solids, num_solids, 1, units, 1, 1, acc,  
            mass_props, stats))) return;  
        write_double_to_listing_window("Surface Area", mass_props[0]);  
        write_double_to_listing_window("Surface Area error", stats[0]);  
        write_double_to_listing_window("Volume", mass_props[1]);  
        write_double_to_listing_window("Volume error", stats[1]);  
        write_double_to_listing_window("Mass", mass_props[2]);  
        write_double_to_listing_window("Mass error", stats[2]);  
        write_double_array_to_listing_window("Center of Mass, WCS",  
            &mass_props[3], 1, 3);  
        write_double_to_listing_window("Center of Mass error sphere radius",  
            stats[3]);  
        write_double_array_to_listing_window("First Moments, Centroidal",  
            &mass_props[6], 1, 3);  
        write_double_array_to_listing_window("Moments errors",  
            &stats[4], 1, 3);  
        write_double_array_to_listing_window("Moments of Inertia, WCS",  
            &mass_props[9], 1, 3);  
        write_double_array_to_listing_window("Moments of Inertia, WCS errors",  
            &stats[7], 1, 3);  
        write_double_array_to_listing_window("Moments of Inertia, Centroidal",  
            &mass_props[12], 1, 3);  
        write_double_to_listing_window("Spherical Moment of inertia",  
            mass_props[15]);  
        write_double_array_to_listing_window("Inertia Products, WCS",  
            &mass_props[16], 1, 3);  
        write_double_array_to_listing_window("Inertia Products, WCS errors",  
            &stats[10], 1, 3);  
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
    static void do_it(void)  
    {  
        int  
            num_solids = 0;  
        tag_t  
            *solids;  
        while ((num_solids = select_bodies("Report Mass Properties", &solids)) > 0)  
            report_mass_properties(num_solids, solids);  
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

> 这段代码是一个用于NX CAD系统的二次开发程序，其主要功能是选择实体，并报告所选实体的质量属性。
>
> 代码的主要功能如下：
>
> 1. 定义了一个错误报告函数report_error，用于在发生UF函数调用错误时输出错误信息。
> 2. 定义了一个过滤函数filter_body_type，用于在实体选择时过滤出指定类型的实体。
> 3. 定义了一个设置实体选择掩码的函数mask_for_bodies。
> 4. 定义了一个用于选择实体的函数select_bodies。
> 5. 定义了用于在列表窗口输出双精度数组和单个双精度数的函数。
> 6. 定义了用于报告实体质量属性的函数report_mass_properties。
> 7. 主函数do_it用于不断选择实体，并报告所选实体的质量属性。
> 8. ufusr函数是程序的入口函数，调用do_it函数。
> 9. ufusr_ask_unload函数用于在程序卸载时立即清理资源。
>
> 总的来说，这段代码通过选择实体，并调用UF函数计算和报告实体的质量属性，如体积、质量、惯性矩等。通过二次开发，扩展了NX系统的功能。
>
