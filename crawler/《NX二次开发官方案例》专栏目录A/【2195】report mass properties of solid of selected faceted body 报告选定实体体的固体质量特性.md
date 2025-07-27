### 【2195】report mass properties of solid of selected faceted body 报告选定实体体的固体质量特性

#### 代码

```cpp
    /*HEAD REPORT_MASS_PROPERTIES_OF_SOLID_OF_SELECTED_FACETED_BODY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_facet.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据您提供的文档，翻译内容如下：

注意：UF_print_syslog 是 V18 中的新增功能，只回答翻译，不要添加无关内容。 */  
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
    /* qq3123197280 */  
    static int mask_for_faceted_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_faceted_model_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_faceted_body(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select faceted body", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faceted_bodies, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            fbody,  
            solid;  
        while ((fbody = select_a_faceted_body("Report Mass Properties")) != NULL_TAG)  
        {  
            if(!UF_CALL(UF_FACET_ask_solid_of_model(fbody, &solid)))  
                report_mass_properties(solid);  
            else  
                uc1601("Solid of faceted model not available", TRUE);  
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

> 这段NX二次开发代码的主要功能是计算并报告选中facet体对应实体的质量属性，并包括以下关键点：
>
> 1. 定义了一个错误报告函数report_error，用于在发生错误时打印错误信息。
> 2. 定义了一个写入单精度数到列表窗口的函数write_double_to_listing_window。
> 3. 定义了一个写入双精度数组到列表窗口的函数write_double_array_to_listing_window。
> 4. 定义了一个报告质量属性的函数report_mass_properties，该函数计算实体的表面面积、体积、质量、质心、惯性矩等，并写入到列表窗口。
> 5. 定义了一个用于facet体的选择掩码函数mask_for_faceted_bodies。
> 6. 定义了一个选择facet体的函数select_a_faceted_body。
> 7. 定义了一个主函数do_it，用于循环选择facet体，计算对应实体的质量属性并报告。
> 8. 定义了ufusr函数，初始化并调用主函数，在结束时关闭NX。
> 9. 定义了ufusr_ask_unload函数，用于立即卸载用户函数。
>
> 该代码实现了选择facet体，计算对应实体的质量属性，并将结果报告到列表窗口的功能。
>
