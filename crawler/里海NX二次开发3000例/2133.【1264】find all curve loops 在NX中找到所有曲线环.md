### 【1264】find all curve loops 在NX中找到所有曲线环

#### 代码

```cpp
    /*HEAD FIND_ALL_CURVE_LOOPS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_vec.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能。 */  
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
    static int ask_all_curves(tag_t part, tag_t **curves)  
    {  
        int  
            ii,  
            types[4] = { UF_line_type,  
                         UF_circle_type,  
                         UF_conic_type,  
                         UF_spline_type };  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        for (ii = 0; ii < 4; ii++)  
            while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, types[ii], &object))  
                && (object != NULL_TAG))  
            UF_CALL(UF_MODL_put_list_item(object_list, object));  
        return (make_an_array(&object_list, curves));  
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
    static void set_highlight_string_list(UF_STRING_t *string, int flag)  
    {  
        int ii, jj, kk = 0;  
        for (ii = 0; ii < string->num; ii++)  
            for (jj = 0; jj < string->string[ii]; jj++, kk++)  
                UF_CALL(UF_DISP_set_highlight(string->id[kk], flag));  
    }  
    static void do_it(void)  
    {  
        int  
            n_curves;  
        tag_t  
            *curves,  
            part = UF_PART_ask_display_part();  
        UF_STRING_t  
            loops;  
        char  
            msg[133];  
        if ((n_curves = ask_all_curves(part, &curves)) > 0)  
        {  
            sort_into_loops(curves, n_curves, &loops);  
            show_string_list_objects(&loops);  
            set_highlight_string_list(&loops, TRUE);  
            sprintf(msg, "%d loops highlighted", loops.num);  
            uc1601(msg, TRUE);  
            set_highlight_string_list(&loops, FALSE);  
            UF_DISP_refresh();  
            UF_MODL_free_string_list(&loops);  
            UF_free(curves);  
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

> 这段代码是用于NX的二次开发，主要功能是找出模型中的所有曲线环，并将它们高亮显示。
>
> 代码的主要步骤包括：
>
> 1. 获取当前显示部件的所有曲线，包括直线、圆弧、圆锥曲线和样条线。
> 2. 对曲线进行排序，形成连续的环。这需要检查每条曲线的端点，找到首尾相连的曲线，将它们组合成环。
> 3. 高亮显示每个曲线环，并在每个环的第一个曲线上显示序号。
> 4. 判断曲线的方向，并在每个曲线的起点或终点显示一个红色箭头，表示曲线的方向。
> 5. 提供一个函数，可以打开或关闭曲线环的高亮显示。
> 6. 在函数内部，使用错误报告宏来检查函数调用是否成功。
> 7. 提供卸载函数，以便在退出时释放内存。
>
> 总体来说，这段代码利用NX的API实现了找出模型中所有曲线环的功能，并对它们进行高亮显示和方向指示。
>
