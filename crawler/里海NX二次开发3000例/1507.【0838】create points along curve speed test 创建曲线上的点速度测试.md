### 【0838】create points along curve speed test 创建曲线上的点速度测试

#### 代码

```cpp
    /*HEAD CREATE_POINTS_ALONG_CURVE_SPEED_TEST CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_so.h>  
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
    static logical prompt_for_two_integers(char *prompt, char *item1, char *item2,  
        int *number1, int *number2)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[2][16];  
        int  
            da[2];  
        strncpy(&menu[0][0], item1, 15);  
        strncpy(&menu[1][0], item2, 15);  
        menu[0][15] = '\0';  
        menu[1][15] = '\0';  
        da[0] = *number1;  
        da[1] = *number2;  
        resp = uc1607(prompt, menu, 2, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = da[0];  
            *number2 = da[1];  
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
        return n_points = make_an_array(&point_list, points);  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        sprintf(msg, "%s = %f\n", title, n);  
        ECHO(msg);  
    }  
    #include <uf_undo.h>  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n = 10,  
            n_pts = 100;  
        tag_t  
            curve,  
            *pts;  
        UF_timer_t  
            timer;  
        UF_timer_values_t  
            times;  
        while (((curve = select_a_curve_or_edge("Points along curve")) != NULL_TAG)  
            && prompt_for_two_integers("How many?", "no. points", "no. times",  
                &n_pts, &n))  
        {  
            set_undo_mark("Create points on curve");  
            UF_CALL(UF_begin_timer(&timer));  
            for (ii = 0; ii < n; ii++)  
            {  
                create_points_spaced_along_curve(curve, 100/(n_pts-1),  
                    UF_SO_point_along_curve_percent, &pts);  
            }  
            UF_CALL(UF_end_timer(timer, &times));  
            WRITE_F(times.cpu_time);  
            WRITE_F(times.real_time);  
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

> 这段代码是用于在NX中创建沿曲线等距点集的二次开发代码，其主要功能和流程如下：
>
> 1. 定义了一些宏，用于错误报告和信息输出。
> 2. 实现了一个函数mask_for_curves_and_edges，用于设置选择集，只选择曲线和边。
> 3. 实现了一个函数select_a_curve_or_edge，用于提示用户选择一个曲线或边。
> 4. 实现了一个函数prompt_for_two_integers，用于提示用户输入两个整数。
> 5. 实现了一个函数allocate_memory，用于申请内存。
> 6. 实现了一个函数make_an_array，用于将列表转换为数组。
> 7. 实现了一个函数create_points_spaced_along_curve，用于在曲线上创建等距的点集。
> 8. 实现了一个函数write_double_to_listing_window，用于输出双精度数。
> 9. 实现了一个函数set_undo_mark，用于设置撤销标记。
> 10. 实现了函数do_it，循环提示用户选择曲线，并创建指定数量的点集，记录运行时间。
> 11. 实现了ufusr函数，初始化NX，调用do_it函数，然后终止NX。
> 12. 实现了ufusr_ask_unload函数，用于卸载模块。
>
> 综上所述，这段代码通过交互方式让用户选择曲线，然后沿曲线创建等距点集，并记录运行时间，用于性能测试。
>
