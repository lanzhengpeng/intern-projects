### 【0911】create smart points at equal distances along curve 在曲线上等距创建智能点

#### 代码

```cpp
    /*HEAD CREATE_SMART_POINTS_AT_EQUAL_DISTANCES_ALONG_CURVE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_so.h>  
    #include <uf_obj.h>  
    #include <uf_curve.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：注意，UF_print_syslog是V18版本新增的，请只回答翻译，不要添加其他内容。 */  
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
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    extern tag_t select_a_curve(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
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
    static void create_points_at_distance_along_curve( tag_t curve, double dist )  
    {  
        tag_t  
            last = NULL_TAG,  
            offset = NULL_TAG,  
            start = NULL_TAG,  
            dist_point = NULL_TAG;  
        double  
            total_lgth = 0.0,  
            current_dist = dist;  
        UF_CALL( UF_CURVE_ask_arc_length ( curve, 0.0, 1.0,   
                                           UF_MODL_UNITS_PART, &total_lgth ));  
        UF_CALL( UF_SO_create_scalar_double(curve, UF_SO_update_after_modeling,  
                                                                   0.0, &start));  
        UF_CALL(UF_SO_create_point_on_curve(curve, UF_SO_update_after_modeling,  
                                                           curve, start, &last));  
        UF_CALL(UF_SO_set_visibility_option(last, UF_SO_invisible));  
        while( current_dist <= total_lgth )  
        {  
            UF_CALL(UF_SO_create_scalar_double(curve, UF_SO_update_after_modeling,  
                                                          current_dist, &offset ));  
            UF_CALL( UF_SO_create_point_along_curve(curve,  
                 UF_SO_update_after_modeling, curve, last, offset,  
                 UF_SO_point_along_curve_distance, FALSE, &dist_point));  
            UF_CALL(UF_SO_set_visibility_option( dist_point, UF_SO_visible ));  
            current_dist += dist;  
            if( dist == 0. ) return; /* otherwise this runs   
                                      forever for a zero point */  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            curve;  
        double  
            ip = 10.0;  
        while (((curve = select_a_curve("Create points along")) != NULL_TAG)  
            && prompt_for_a_number("Distance Along Curve:", "Distance:", &ip))  
            create_points_at_distance_along_curve(curve, ip);  
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

> 这段NX Open C++二次开发代码的功能是沿曲线创建等距点。
>
> 主要步骤包括：
>
> 1. 包含必要的头文件。
> 2. 定义错误报告函数report_error，用于打印错误信息。
> 3. 定义mask_for_curves函数，设置选择过滤器，只允许选择线、圆、圆锥曲线和样条曲线。
> 4. 定义select_a_curve函数，用于选择一条曲线。
> 5. 定义prompt_for_a_number函数，用于提示用户输入等距距离。
> 6. 定义create_points_at_distance_along_curve函数，用于沿曲线创建等距点。
> 7. 定义do_it函数，用于循环选择曲线并创建等距点。
> 8. 定义ufusr函数，用于初始化NX Open并调用do_it函数。
> 9. 定义ufusr_ask_unload函数，用于卸载NX Open。
>
> 通过选择曲线和输入等距距离，该代码可以沿曲线创建一系列等距点，实现了曲线等距点的创建功能。
>
