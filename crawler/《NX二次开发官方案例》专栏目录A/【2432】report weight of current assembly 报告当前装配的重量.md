### 【2432】report weight of current assembly 报告当前装配的重量

#### 代码

```cpp
    /*HEAD REPORT_WEIGHT_OF_CURRENT_ASSEMBLY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <math.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_weight.h>  
    #include <uf_cfi.h>  
    #include <uf_obj.h>  
    #include <uf_assem.h>  
    static void ECHO(char* format, ...)  
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
    static int report_error(char* file, int line, char* call, int irc)  
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
    static void report_weight_units(char* name, UF_WEIGHT_units_type_t units)  
    {  
        switch (units)  
        {  
        case UF_WEIGHT_units_km:  
            ECHO("%s = UF_WEIGHT_units_km (Kilograms and meters)\n", name);  
            break;  
        case UF_WEIGHT_units_li:  
            ECHO("%s = UF_WEIGHT_units_li (Pounds and inches)\n", name);  
            break;  
        case UF_WEIGHT_units_lf:  
            ECHO("%s = UF_WEIGHT_units_lf (Pounds and feet)\n", name);  
            break;  
        case UF_WEIGHT_units_gm:  
            ECHO("%s = UF_WEIGHT_units_gm (Grams and millimeters)\n", name);  
            break;  
        case UF_WEIGHT_units_gc:  
            ECHO("%s = UF_WEIGHT_units_gc (Grams and centimeters)\n", name);  
            break;  
        case UF_WEIGHT_units_kmm:  
            ECHO("%s = UF_WEIGHT_units_kmm (Kilograms and millimeters)\n", name);  
            break;  
        default:  
            ECHO("%s = %d? (unknown)\n", name, units);  
        }  
    }  
    static void report_weight_state(char* name, UF_WEIGHT_state_type_t state)  
    {  
        switch (state)  
        {  
        case UF_WEIGHT_no_cache:  
            ECHO("%s = UF_WEIGHT_no_cache (no data available)\n", name);  
            break;  
        case UF_WEIGHT_cached:  
            ECHO("%s = UF_WEIGHT_cached (calculated)\n", name);  
            break;  
        case UF_WEIGHT_asserted:  
            ECHO("%s = UF_WEIGHT_asserted (explicit)\n", name);  
            break;  
        case UF_WEIGHT_unknown:  
            ECHO("%s = UF_WEIGHT_unknown (explicitly unknown)\n", name);  
            break;  
        case UF_WEIGHT_inherited:  
            ECHO("%s = UF_WEIGHT_inherited (from assertion on part)\n", name);  
            break;  
        case UF_WEIGHT_implied:  
            ECHO("%s = UF_WEIGHT_implied (derived)\n", name);  
            break;  
        default:  
            ECHO("%s = %d? (unknown)\n", name, state);  
        }  
    }  
    static void report_weight_properties(UF_WEIGHT_properties_t* properties)  
    {  
        report_weight_units("properties->units", properties->units);  
        report_weight_state("properties->cache_state", properties->cache_state);  
        if (properties->cache_state == UF_WEIGHT_no_cache) return;  
        ECHO("properties->accuracy = %f\n", properties->accuracy);  
        ECHO("properties->density = %f\n", properties->density);  
        ECHO("properties->volume = %f\n", properties->volume);  
        ECHO("properties->volume_error = %f\n", properties->volume_error);  
        report_weight_state("properties->volume_state", properties->volume_state);  
        ECHO("properties->mass = %f\n", properties->mass);  
        ECHO("properties->mass_error = %f\n", properties->mass_error);  
        report_weight_state("properties->mass_state", properties->mass_state);  
        ECHO("properties->area = %f\n", properties->area);  
        ECHO("properties->area_error = %f\n", properties->area_error);  
        report_weight_state("properties->area_state", properties->area_state);  
        ECHO("properties->center_of_mass = %f\n", properties->center_of_mass);  
        ECHO("properties->cofm_error = %f\n", properties->cofm_error);  
        report_weight_state("properties->cofm_state", properties->cofm_state);  
        ECHO("properties->moments_of_inertia = %f\n", properties->moments_of_inertia);  
        ECHO("Radius of Gyration (Centroidal)\n");  
        if (properties->mass > 0)  
        {  
            ECHO("rgx = %f\n", sqrt(properties->moments_of_inertia[0] / properties->mass));  
            ECHO("rgy = %f\n", sqrt(properties->moments_of_inertia[1] / properties->mass));  
            ECHO("rgz = %f\n", sqrt(properties->moments_of_inertia[2] / properties->mass));  
        }  
        ECHO("properties->products_of_inertia = %f\n", properties->products_of_inertia);  
        ECHO("properties->mofi_error = %f\n", properties->mofi_error);  
        report_weight_state("properties->mofi_state", properties->mofi_state);  
    }  
    static void report_weight_exception_reason(char* name,  
        UF_WEIGHT_exception_reason_t reason)  
    {  
        switch (reason)  
        {  
        case UF_WEIGHT_not_loaded:  
            ECHO("%s = UF_WEIGHT_not_loaded\n", name);  
            break;  
        case UF_WEIGHT_insufficiently_loaded:  
            ECHO("%s = UF_WEIGHT_insufficiently_loaded\n", name);  
            break;  
        case UF_WEIGHT_ref_set_absent:  
            ECHO("%s = UF_WEIGHT_ref_set_absent\n", name);  
            break;  
        case UF_WEIGHT_comp_set_absent:  
            ECHO("%s = UF_WEIGHT_comp_set_absent\n", name);  
            break;  
        case UF_WEIGHT_under_minimum_weight:  
            ECHO("%s = UF_WEIGHT_under_minimum_weight\n", name);  
            break;  
        case UF_WEIGHT_over_maximum_weight:  
            ECHO("%s = UF_WEIGHT_over_maximum_weight\n", name);  
            break;  
        case UF_WEIGHT_has_assertion:  
            ECHO("%s = UF_WEIGHT_has_assertion\n", name);  
            break;  
        case UF_WEIGHT_insufficient_accuracy:  
            ECHO("%s = UF_WEIGHT_insufficient_accuracy\n", name);  
            break;  
        case UF_WEIGHT_incomplete_part:  
            ECHO("%s = UF_WEIGHT_incomplete_part\n", name);  
            break;  
        case UF_WEIGHT_comp_group_not_uptodate:  
            ECHO("%s = UF_WEIGHT_comp_group_not_uptodate\n", name);  
            break;  
        case UF_WEIGHT_comp_group_updated_successfully:  
            ECHO("%s = UF_WEIGHT_comp_group_updated_successfully\n", name);  
            break;  
        case UF_WEIGHT_reference_only_component:  
            ECHO("%s = UF_WEIGHT_reference_only_component\n", name);  
            break;  
        case UF_WEIGHT_excluded_from_spatial_search:  
            ECHO("%s = UF_WEIGHT_excluded_from_spatial_search\n", name);  
            break;  
        case UF_WEIGHT_promotion_with_source_suppressed:  
            ECHO("%s = UF_WEIGHT_promotion_with_source_suppressed\n", name);  
            break;  
        default:  
            ECHO("%s = %d? (unknown)\n", name, reason);  
        }  
    }  
    static void report_weight_exceptions(UF_WEIGHT_exceptions_t * exceptions)  
    {  
        int  
            ii;  
        UF_UI_open_listing_window();  
        ECHO("exceptions->n_exceptions = %d\n", exceptions->n_exceptions);  
        for (ii = 0; ii < exceptions->n_exceptions; ii++)  
        {  
            ECHO("#%d\n", ii);  
            report_weight_exception_reason("exceptions->reasons[ii]", exceptions->reasons[ii]);  
            ECHO("exceptions->part_names[ii] = %s\n", exceptions->part_names[ii]);  
            ECHO("exceptions->comp_names[ii] = %s\n", exceptions->comp_names[ii]);  
            ECHO("exceptions->on_part[ii] = %d\n", exceptions->on_part[ii]);  
            ECHO("exceptions->repeat_counts[ii] = %d\n", exceptions->repeat_counts[ii]);  
        }  
        UF_CALL(UF_WEIGHT_free_exceptions(exceptions));  
    }  
    static tag_t ask_current_cset(void)  
    {  
        tag_t  
            cset = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_OBJ_cycle_by_name_and_type(part, "CurrentComponents",  
            UF_component_set_type, FALSE, &cset));  
        return cset;  
    }  
    static logical remove_component_weight_assertion(tag_t component, void* data)  
    {  
        /*  Do not UF_CALL this to avoid error raise if component does not have  
            an applied weight assertion.  
        */  
        UF_WEIGHT_delete_comp_assertion(component);  
        return TRUE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part();  
        UF_WEIGHT_exceptions_t  
            exceptions;  
        UF_WEIGHT_properties_t  
            properties;  
        UF_CALL(UF_ASSEM_apply_to_cset(ask_current_cset(),  
            remove_component_weight_assertion, NULL));  
        UF_CALL(UF_WEIGHT_set_part_cset(part, "CurrentComponents"));  
        UF_CALL(UF_WEIGHT_set_part_ref_set(part, "Entire Part"));  
        UF_CALL(UF_WEIGHT_init_exceptions(&exceptions));  
        UF_CALL(UF_WEIGHT_estab_part_props(part, 0.9, TRUE, UF_WEIGHT_units_li,  
            &properties, &exceptions));  
        report_weight_properties(&properties);  
        report_weight_exceptions(&exceptions);  
    }  
    /* qq3123197280 */  
    void ufusr(char* param, int* retcode, int paramLen)  
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
        for (ii = 0; ii < status->n_parts; ii++)  
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
    int main(int argc, char* argv[])  
    {  
        int  
            cnt = 0;  
        tag_t  
            part;  
        char  
            part_name[MAX_FSPEC_SIZE + 1];  
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

> 这段代码是一个NX Open Application的示例，主要功能是计算和报告当前组件集的重量属性。
>
> 代码的主要功能包括：
>
> 1. 删除当前组件集中的重量断言，以便重新计算重量。
> 2. 设置当前组件集和参考集为整个零件，以便计算整个零件的重量。
> 3. 使用UF_WEIGHT_estab_part_props函数计算当前组件集的重量属性，包括质量、体积、面积、惯性矩等。
> 4. 报告计算出的重量属性，包括单位、计算状态、准确度、密度、体积、质量、面积、质心位置、惯性矩等。
> 5. 报告计算过程中出现的异常，包括异常原因、零件名称、组件名称、是否在零件上、重复次数等。
> 6. 在命令行模式下，可以指定要处理的零件路径；在UG/Manager模式下，可以指定要处理的零件号。
> 7. 在打开每个零件后，会检查加载状态，并报告加载失败的零件。
> 8. 如果零件加载成功，会计算并报告重量属性，然后关闭所有零件。
> 9. 在程序结束前，会报告共处理了多少个零件。
>
> 这段代码通过NX Open Application的形式实现了对NX零件重量属性的自动计算和报告，可以用于重量分析、校验等目的。
>
