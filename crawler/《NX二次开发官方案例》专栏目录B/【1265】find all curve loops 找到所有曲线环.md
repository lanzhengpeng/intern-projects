### 【1265】find all curve loops 找到所有曲线环

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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据注释，翻译如下：

翻译 NOTE:  UF_print_syslog是在V18版本中新增的，请只回答翻译，不要添加其他无关内容。 */  
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

> 这段NX Open C++代码的主要功能是：
>
> 1. 查询和排序：它首先查询当前显示部件中的所有曲线，并将它们按照首尾相连的顺序进行排序，形成闭合的曲线环。
> 2. 显示和突出显示：然后，它会显示每个曲线环中的曲线，并突出显示每个曲线环。具体来说，它会根据曲线的首尾方向，用红色箭头显示曲线的方向，并在每个曲线上显示编号。同时，它会高亮显示每个曲线环。
> 3. 交互：用户可以通过点击"OK"来取消曲线环的高亮显示。
> 4. 错误处理：代码中包含了错误处理机制，可以输出错误信息。
> 5. 内存管理：代码还包含了内存管理函数，用于分配和释放内存。
> 6. 注释：代码中使用了中文注释，以方便阅读和理解。
> 7. NX API调用：代码调用了NX提供的几何建模API，包括查询、排序、显示等。
> 8. NX Open C++语法：代码遵循了NX Open C++的语法规范。
> 9. NX Open C++库函数：代码使用了NX Open C++提供的库函数，如UF_MODL、UF_OBJ、UF_DISP等。
> 10. NX Open C++数据结构：代码使用了NX Open C++的数据结构，如uf_list_p_t、tag_t等。
>
> 总的来说，这段代码实现了NX部件中所有曲线的查询、排序和显示，以及交互式突出显示曲线环的功能，是一个典型的NX Open C++二次开发示例。
>
