### 【1110】create bounded plane 创建边界平面

#### 代码

```cpp
    /*HEAD CREATE_BOUNDED_PLANE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本新增的功能，主要用于在系统日志中记录信息。 */  
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
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, UF_all_subtype, 0 },  
                        { UF_spline_type, 0, 0 },  
                      };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_curves(char *prompt, tag_t **curves)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select curves", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_curves, NULL, &resp, &cnt, curves));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*curves)[ii], FALSE));  
        return cnt;  
    }  
    static void ask_curve_ends(tag_t curve, double start[3], double end[3])  
    {  
        double  
            junk[3];  
        UF_CALL(UF_MODL_ask_curve_props(curve, 0.0, start, junk, junk, junk,  
            junk, junk));  
        UF_CALL(UF_MODL_ask_curve_props(curve, 1.0, end, junk, junk, junk,  
            junk, junk));  
    }  
    static void sort_into_loops(tag_t *curves, int n_curves, UF_STRING_p_t string)  
    {  
        logical  
            found;  
        int  
            dirs[100],  
            ii,  
            jj,  
            kk,  
            n_segs[100],  
            n_strings,  
            start;  
        double  
            free_start[3],  
            free_end[3],  
            gap,  
            this_start[3],  
            this_end[3],  
            tol;  
        tag_t  
            temp;  
        UF_MODL_ask_distance_tolerance(&tol);  
        n_strings = 0;  
        n_segs[n_strings] = 1;  
        dirs[n_strings] = UF_MODL_CURVE_START_FROM_BEGIN;  
        ask_curve_ends(curves[0], free_start, free_end);  
        start = 0;  
        for (ii = 0; ii < n_curves; ii++)  
        {  
            found = FALSE;  
            if (!UF_MODL_ask_curve_closed(curves[ii]))  
            {  
                for (jj = ii + 1; jj < n_curves; jj++)  
                {  
                    ask_curve_ends(curves[jj], this_start, this_end);  
                    UF_VEC3_distance(this_start, free_start, &gap);  
                    if (gap <= tol)  
                    {  
                        temp = curves[jj];  
                        for (kk = jj; kk > start; kk--)  
                            curves[kk] = curves[kk-1];  
                        curves[start] = temp;  
                        n_segs[n_strings]++;  
                        dirs[n_strings] = UF_MODL_CURVE_START_FROM_END;  
                        UF_VEC3_copy(this_end, free_start);  
                        found = TRUE;  
                        break;  
                    }  
                    UF_VEC3_distance(this_end, free_start, &gap);  
                    if (gap <= tol)  
                    {  
                        temp = curves[jj];  
                        for (kk = jj; kk > start; kk--)  
                            curves[kk] = curves[kk-1];  
                        curves[start] = temp;  
                        n_segs[n_strings]++;  
                        dirs[n_strings] = UF_MODL_CURVE_START_FROM_BEGIN;  
                        UF_VEC3_copy(this_start, free_start);  
                        found = TRUE;  
                        break;  
                    }  
                    UF_VEC3_distance(this_start, free_end, &gap);  
                    if (gap <= tol)  
                    {  
                        temp = curves[jj];  
                        curves[jj] = curves[ii+1];  
                        curves[ii+1] = temp;  
                        n_segs[n_strings]++;  
                        dirs[n_strings] = UF_MODL_CURVE_START_FROM_BEGIN;  
                        UF_VEC3_copy(this_end, free_end);  
                        found = TRUE;  
                        break;  
                    }  
                    UF_VEC3_distance(this_end, free_end, &gap);  
                    if (gap <= tol)  
                    {  
                        temp = curves[jj];  
                        curves[jj] = curves[ii+1];  
                        curves[ii+1] = temp;  
                        n_segs[n_strings]++;  
                        dirs[n_strings] = UF_MODL_CURVE_START_FROM_BEGIN;  
                        UF_VEC3_copy(this_start, free_end);  
                        found = TRUE;  
                        break;  
                    }  
                }  
                UF_VEC3_distance(free_start, free_end, &gap);  
            }  
            else gap = 0;  
            if (found == FALSE)  
            {  
                if (gap <= tol) n_strings++;  
                else  
                {  
                    n_curves = n_curves - n_segs[n_strings];  
                    ii = ii - n_segs[n_strings];  
                    for (jj = start; jj < n_curves; jj++)  
                        curves[jj] = curves[jj + n_segs[n_strings]];  
                }  
                n_segs[n_strings] = 1;  
                dirs[n_strings] = UF_MODL_CURVE_START_FROM_BEGIN;  
                start = ii+1;  
                if (start < n_curves)  
                    ask_curve_ends(curves[start], free_start, free_end);  
            }  
        }  
        UF_MODL_init_string_list(string);  
        UF_MODL_create_string_list(n_strings, n_curves, string);  
        for (ii = 0; ii < n_strings; ii++)  
        {  
            string->string[ii] = n_segs[ii];  
            string->dir[ii] = dirs[ii];  
        }  
        for (ii = 0; ii < n_curves; ii++)  
            string->id[ii] = curves[ii];  
    }  
    static void display_temporary_colored_text(char *text, double loc[3], int color)  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        props.color = color;  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_ACTIVE_MINUS,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static void display_colored_conehead(double origin[3], double vec[3], int color)  
    {  
        UF_DISP_conehead_attrb_s  
            attrb;  
        UF_DISP_get_conehead_attrb(&attrb);  
        attrb.color = color;  
        UF_DISP_set_conehead_attrb(&attrb);  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);  
        UF_DISP_reset_conehead_attrb();  
    }  
    static void show_curve_direction(tag_t curve)  
    {  
        double  
            junk[3],  
            start[3],  
            tangent[3];  
        UF_CALL(UF_MODL_ask_curve_props(curve, 0.0, start, tangent, junk, junk,  
                junk, junk));  
        display_colored_conehead(start, tangent, UF_OBJ_RED);  
    }  
    static void show_curve_flip_direction(tag_t curve)  
    {  
        double  
            junk[3],  
            end[3],  
            flip_dir[3],  
            tangent[3];  
        UF_CALL(UF_MODL_ask_curve_props(curve, 1.0, end, tangent, junk, junk,  
                junk, junk));  
        UF_VEC3_negate(tangent, flip_dir);  
        display_colored_conehead(end, flip_dir, UF_OBJ_RED);  
    }  
    static void show_string_list_objects(UF_STRING_t *string)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        double  
            loc[3];  
        char  
            msg[133];  
        for (ii = 0; ii < string->num; ii++)  
        {  
            if (string->dir[ii] == UF_MODL_CURVE_START_FROM_BEGIN)  
                show_curve_direction(string->id[kk]);  
            else  
                show_curve_flip_direction(string->id[kk]);  
            for (jj = 0; jj < string->string[ii]; jj++)  
            {  
                sprintf(msg, "%d", jj+1);  
                if (UF_OBJ_ask_name_origin(string->id[kk], loc))  
                {  
                    UF_CALL(UF_OBJ_set_name(string->id[kk], "X"));  
                    UF_CALL(UF_OBJ_ask_name_origin(string->id[kk], loc));  
                    UF_CALL(UF_OBJ_delete_name(string->id[kk]));  
                }  
                display_temporary_colored_text(msg, loc, UF_OBJ_WHITE);  
                kk++;  
            }  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            n_curves;  
        tag_t  
            bplane,  
            *curves;  
        double  
            tol[3];  
        UF_STRING_t  
            s_section;  
        UF_MODL_ask_distance_tolerance(&tol[0]);  
        UF_MODL_ask_angle_tolerance(&tol[1]);  
        while ((n_curves = select_curves("Create Bounded Plane", &curves)) > 0)  
        {  
            UF_DISP_refresh();  
            sort_into_loops(curves, n_curves, &s_section);  
            show_string_list_objects(&s_section);  
            UF_CALL(UF_MODL_create_bplane(&s_section, tol, &bplane));  
            UF_MODL_free_string_list(&s_section);  
            UF_free(curves);  
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

> 这段代码是用于NX的二次开发代码，其主要功能是创建一个受曲线限制的平面。
>
> 关键功能包括：
>
> 1. 提供了错误报告函数report_error，用于在出现UF调用错误时记录错误信息。
> 2. 实现了mask_for_curves函数，用于设置选择曲线时的类型掩码。
> 3. 实现了select_curves函数，用于弹出选择对话框，让用户选择曲线。
> 4. 实现了ask_curve_ends函数，用于获取曲线的起点和终点。
> 5. 实现了sort_into_loops函数，用于将选择的曲线排序成封闭的环。
> 6. 实现了show_curve_direction和show_curve_flip_direction函数，用于显示曲线的方向和翻转方向。
> 7. 实现了show_string_list_objects函数，用于显示排序后的曲线。
> 8. 实现了do_it函数，用于完成创建受曲线限制的平面的主流程。
> 9. 实现了ufusr和ufusr_ask_unload函数，分别用于初始化和卸载NX Open。
>
> 总的来说，这段代码实现了创建受曲线限制的平面的功能，包括选择曲线、排序曲线、显示方向，并最终创建出受曲线限制的平面。
>
