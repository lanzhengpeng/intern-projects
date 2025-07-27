### 【0845】create points spaced along selected curve or edge 在选定曲线或边沿线等距创建点

#### 代码

```cpp
    /*HEAD CREATE_SMART_POINTS_SPACED_ALONG_CURVE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_so.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，可以总结如下：

1. UF_print_syslog 是 V18 版本新增的函数。

2. 该函数用于将日志信息打印到系统日志。

3. UF_print_syslog() 函数是 V18 新增的，用于将日志信息打印到系统日志。

4. UF_print_syslog() 函数是 V18 新增的，用于将日志信息打印到系统日志。

5. UF_print_syslog() 函数是 V18 新增的，用于将日志信息打印到系统日志。

6. UF_print_syslog() 函数是 V18 新增的，用于将日志信息打印到系统日志。

7. UF_print_syslog() 函数是 V18 新增的，用于将日志信息打印到系统日志。

8. UF_print_syslog() 函数是 V18 新增的，用于将日志信息打印到系统日志。

9. UF_print_syslog() 函数是 V18 新增的，用于将日志信息打印到系统日志。

10. UF_print_syslog() 函数是 V18 新增的，用于将日志信息打印到系统日志。 */  
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
    /* 里海 */  
    static int mask_for_curves_and_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[5] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 5, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    extern tag_t select_a_curve_or_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve or edge", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves_and_edges, NULL,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical pick_one_of_two_choices(char *prompt, char *option_one,  
        char *option_two, int *choice)  
    {  
        int  
            resp;  
        char  
            options[2][38];  
        strncpy(&options[0][0], option_one, 37);  
        strncpy(&options[1][0], option_two, 37);  
        options[0][37] = '\0';  
        options[1][37] = '\0';  
        resp = uc1603(prompt, *choice, options, 2);  
        if ((resp > 4) && (resp < 19))  
        {  
            *choice = resp - 4;  /* returns 1 or 2 译:返回1或2。 */  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static int create_points_spaced_along_curve(tag_t curve, double increment,  
        UF_SO_point_along_curve_option_t dist_vs_pct, tag_t **points)  
    {  
        int  
            n_points;  
        tag_t  
            last,  
            offset,  
            start;  
        double  
            step,  
            total_length;  
        uf_list_p_t  
            point_list;  
        UF_CALL(UF_MODL_create_list(&point_list));  
        UF_CALL(UF_SO_create_scalar_double(curve, UF_SO_update_after_modeling,  
            0.0, &start));  
        UF_CALL(UF_SO_create_point_on_curve(curve, UF_SO_update_after_modeling,  
            curve, start, &last));  
        UF_CALL(UF_SO_set_visibility_option(last, UF_SO_visible));  
        UF_CALL(UF_MODL_put_list_item(point_list, last));  
        switch (dist_vs_pct)  
        {  
            case UF_SO_point_along_curve_distance:  
                UF_CALL(UF_SO_create_scalar_double(curve,  
                    UF_SO_update_after_modeling, increment, &offset));  
                UF_CALL(UF_CURVE_ask_arc_length(curve, 0.0, 1.0,  
                    UF_MODL_UNITS_PART, &total_length));  
                for (step = increment; step < total_length; step = step+increment)  
                {  
                    UF_CALL(UF_SO_create_point_along_curve(curve,  
                        UF_SO_update_after_modeling, curve, last, offset,  
                        UF_SO_point_along_curve_distance, FALSE, &last));  
                    UF_CALL(UF_SO_set_visibility_option(last, UF_SO_visible));  
                    UF_CALL(UF_MODL_put_list_item(point_list, last));  
                }  
                break;  
            case UF_SO_point_along_curve_percent:  
                UF_CALL(UF_SO_create_scalar_double(curve,  
                    UF_SO_update_after_modeling, increment/100, &offset));  
                for (step = increment; step <= 100; step = step + increment)  
                {  
                    UF_CALL(UF_SO_create_point_along_curve(curve,  
                        UF_SO_update_after_modeling, curve, last, offset,  
                        UF_SO_point_along_curve_percent, FALSE, &last));  
                    UF_CALL(UF_SO_set_visibility_option(last, UF_SO_visible));  
                    UF_CALL(UF_MODL_put_list_item(point_list, last));  
                }  
                break;  
            default:  
                break;  
        }  
        UF_CALL(UF_MODL_delete_object_parms(point_list));  
        return n_points = make_an_array(&point_list, points);  
    }  
    static void display_temporary_text(char *text, double loc[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static void number_objects(tag_t *objects, int n_objects)  
    {  
        int  
            ii;  
        double  
            loc[3];  
        char  
            msg[133];  
        for (ii = 0; ii < n_objects; ii++)  
        {  
            sprintf(msg, "%d", ii+1);  
            if (UF_OBJ_ask_name_origin(objects[ii], loc))  
            {  
                UF_CALL(UF_OBJ_set_name(objects[ii], "X"));  
                UF_CALL(UF_OBJ_ask_name_origin(objects[ii], loc));  
                UF_CALL(UF_OBJ_delete_name(objects[ii]));  
            }  
            display_temporary_text(msg, loc);  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            mc,  
            method_flags[2] = { UF_SO_point_along_curve_distance,  
                                UF_SO_point_along_curve_percent },  
            n_points;  
        tag_t  
            curve,  
            *points;  
        double  
            incs[2] = { 0.5, 10.0 };  
        char  
            methods[2][38] = { "Arclength", "Percentage" };  
        while (((curve = select_a_curve_or_edge("Create points along")) != NULL_TAG)  
            && pick_one_of_two_choices("Increment Method", methods[0], methods[1],  
                &mc)  
            && prompt_for_a_number("Incremental", methods[mc-1], &incs[mc-1]))  
        {  
            n_points = create_points_spaced_along_curve(curve, incs[mc-1],  
                method_flags[mc-1], &points);  
            number_objects(points, n_points);  
            if (n_points > 0) UF_free(points);  
        }  
    }  
    /* 里海 */  
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

> 这段代码是用于NX二次开发的，主要功能是沿着选定的曲线或边缘创建等间距的点。其主要功能包括：
>
> 1. 提示用户选择曲线或边缘，并为其添加高亮显示。
> 2. 让用户选择是按弧长还是百分比来创建点。
> 3. 提示用户输入增量，并按照选择的增量方式创建点。
> 4. 对创建的点进行编号。
> 5. 在创建的点名称位置显示编号。
> 6. 释放内存并结束NX调用。
>
> 这段代码主要使用了UF、UF_UI、UF_SO、UF_OBJ等NX提供的API来实现这些功能。其结构清晰，注释详细，易于理解和维护。
>
