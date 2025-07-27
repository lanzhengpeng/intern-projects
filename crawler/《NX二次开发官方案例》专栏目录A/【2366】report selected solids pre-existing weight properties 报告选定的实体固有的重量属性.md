### 【2366】report selected solids pre-existing weight properties 报告选定的实体固有的重量属性

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_SOLIDS_PRE-EXISTING_WEIGHT_PROPERTIES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_weight.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog是V18版本中的新功能。 */  
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
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITE9F(X)  (write_double_array_to_listing_window(#X, X, 3, 3))  
    #define WRITE16F(X) (write_double_array_to_listing_window(#X, X, 4, 4))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            UF_CALL(UF_UI_write_listing_window(msg));  
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
            UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
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
    static int mask_body_type( UF_UI_selection_p_t select, void *type)  
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
        UF_CALL(UF_UI_select_with_single_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_body_type, &solid, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void report_weight_units(char *name, UF_WEIGHT_units_type_t units)  
    {  
        char  
            msg[133];  
        switch (units)  
        {  
            case UF_WEIGHT_units_km:  
                sprintf(msg, "%s = UF_WEIGHT_units_km (Kilograms and meters)\n",  
                    name);  
                UF_UI_write_listing_window(msg);  
                break;  
            case UF_WEIGHT_units_li:  
                sprintf(msg, "%s = UF_WEIGHT_units_li (Pounds and inches)\n", name);  
                UF_UI_write_listing_window(msg);  
                break;  
            case UF_WEIGHT_units_lf:  
                sprintf(msg, "%s = UF_WEIGHT_units_lf (Pounds and feet)\n", name);  
                UF_UI_write_listing_window(msg);  
                break;  
            case UF_WEIGHT_units_gm:  
                sprintf(msg, "%s = UF_WEIGHT_units_gm (Grams and millimeters)\n",  
                    name);  
                UF_UI_write_listing_window(msg);  
                break;  
            case UF_WEIGHT_units_gc:  
                sprintf(msg, "%s = UF_WEIGHT_units_gc (Grams and centimeters)\n",  
                    name);  
                UF_UI_write_listing_window(msg);  
                break;  
            default:  
                sprintf(msg, "%s = %d? (unknown)\n", name, units);  
                UF_UI_write_listing_window(msg);  
        }  
    }  
    static void report_weight_state(char *name, UF_WEIGHT_state_type_t state)  
    {  
        char  
            msg[133];  
        switch (state)  
        {  
            case UF_WEIGHT_no_cache:  
                sprintf(msg, "%s = UF_WEIGHT_no_cache (no data available)\n",  
                    name);  
                UF_UI_write_listing_window(msg);  
                break;  
            case UF_WEIGHT_cached:  
                sprintf(msg, "%s = UF_WEIGHT_cached (calculated)\n", name);  
                UF_UI_write_listing_window(msg);  
                break;  
            case UF_WEIGHT_asserted:  
                sprintf(msg, "%s = UF_WEIGHT_asserted (explicit)\n", name);  
                UF_UI_write_listing_window(msg);  
                break;  
            case UF_WEIGHT_unknown:  
                sprintf(msg, "%s = UF_WEIGHT_unknown (explicitly unknown)\n",  
                    name);  
                UF_UI_write_listing_window(msg);  
                break;  
            case UF_WEIGHT_inherited:  
                sprintf(msg, "%s = UF_WEIGHT_inherited (from assertion on part)\n",  
                    name);  
                UF_UI_write_listing_window(msg);  
                break;  
            case UF_WEIGHT_implied:  
                sprintf(msg, "%s = UF_WEIGHT_implied (derived)\n",  
                    name);  
                UF_UI_write_listing_window(msg);  
                break;  
            default:  
                sprintf(msg, "%s = %d? (unknown)\n", name, state);  
                UF_UI_write_listing_window(msg);  
        }  
    }  
    static void report_weight_properties( UF_WEIGHT_properties_t *properties)  
    {  
        UF_UI_open_listing_window();  
        report_weight_units("properties->units", properties->units);  
        report_weight_state("properties->cache_state", properties->cache_state);  
        if (properties->cache_state == UF_WEIGHT_no_cache) return;  
        WRITE_F(properties->accuracy);  
        WRITE_F(properties->density);  
        report_weight_state("properties->density_state", properties->density_state);  
        WRITE_F(properties->volume);  
        WRITE_F(properties->volume_error);  
        report_weight_state("properties->volume_state", properties->volume_state);  
        WRITE_F(properties->mass);  
        WRITE_F(properties->mass_error);  
        report_weight_state("properties->mass_state", properties->mass_state);  
        WRITE_F(properties->area);  
        WRITE_F(properties->area_error);  
        report_weight_state("properties->area_state", properties->area_state);  
        WRITE3F(properties->center_of_mass);  
        WRITE_F(properties->cofm_error);  
        report_weight_state("properties->cofm_state", properties->cofm_state);  
        WRITE3F(properties->moments_of_inertia);  
        WRITE3F(properties->products_of_inertia);  
        WRITE_F(properties->mofi_error);  
        report_weight_state("properties->mofi_state", properties->mofi_state);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            solid;  
        UF_WEIGHT_properties_t  
            properties;  
        while ((solid = select_a_solid("Select solid")) != NULL_TAG)  
        {  
            UF_CALL(UF_WEIGHT_ask_props(solid, UF_WEIGHT_units_li, &properties));  
            report_weight_properties(&properties);  
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

> 这段NX二次开发代码的主要功能是选择一个实体，并报告其质量属性。具体来说：
>
> 1. 用户交互：通过UF_UI_select_with_single_dialog函数，用户可以交互式选择一个实体。
> 2. 质量属性获取：使用UF_WEIGHT_ask_props函数获取选定实体的质量属性，包括密度、体积、质量、面积、质心位置、惯性矩等。
> 3. 质量属性报告：通过write_double_to_listing_window和report_weight_units等函数，将质量属性以文本形式输出到列表窗口。
> 4. 错误处理：使用UF_CALL宏进行错误处理，并通过UF_print_syslog打印错误信息。
> 5. 主函数：在do_it函数中循环选择实体，并报告其质量属性，直到用户选择退出。
> 6. 初始化和终止：在ufusr函数中初始化和终止NX。
> 7. 卸载处理：ufusr_ask_unload函数用于处理NX卸载二次开发程序的请求。
>
> 总的来说，这段代码通过交互方式选择实体，并报告其详细的质量属性信息，主要用于质量分析和校核。
>
