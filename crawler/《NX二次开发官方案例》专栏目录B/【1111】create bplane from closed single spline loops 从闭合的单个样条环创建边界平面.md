### 【1111】create bplane from closed single spline loops 从闭合的单个样条环创建边界平面

#### 代码

```cpp
    /*HEAD CREATE_BOUNDED_PLANES_FROM_CLOSED_SINGLE_SPLINE_LOOPS CCC UFUN */   
    #include <stdio.h>   
    #include <string.h>   
    #include <uf.h>   
    #include <uf_ui.h>   
    #include <uf_object_types.h>   
    #include <uf_modl.h>   
    #include <uf_disp.h>   
    #include <uf_vec.h>   
    #include <uf_obj.h>   
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档描述，UF_print_syslog是V18版本新增的函数。 */   
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
    static int ask_all_of_type(tag_t part, tag_t **objects)   
    {   
        tag_t   
            object = NULL_TAG;   
        uf_list_p_t   
            object_list;   
        UF_CALL(UF_MODL_create_list(&object_list));   
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_spline_type, &object)) &&   
                (object != NULL_TAG))   
            UF_CALL(UF_MODL_put_list_item(object_list, object));   
        return (make_an_array(&object_list, objects));   
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
    static void sort_into_strings(tag_t *curves, int n_curves, UF_STRING_p_t string)   
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
            if (found == FALSE)   
            {   
                n_strings++;   
                n_segs[n_strings] = 1;   
                dirs[n_strings] = UF_MODL_CURVE_START_FROM_BEGIN;   
                start = ii+1;   
                if (ii+1 < n_curves)   
                    ask_curve_ends(curves[ii+1], free_start, free_end);   
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
    static void do_it(void)   
    {   
        int   
            jj,   
            n_curves;   
        tag_t   
            bplane,   
            *objects,   
            part=UF_PART_ask_display_part();   
        double   
            tol[3];   
        UF_STRING_t   
            s_section;   
        UF_MODL_ask_distance_tolerance(&tol[0]);   
        UF_MODL_ask_angle_tolerance(&tol[1]);   
        n_curves= ask_all_of_type(part, &objects);   
        printf("n_curves %d\n", n_curves);   
        for (jj=0; jj < n_curves; jj++)    
        {   
            sort_into_strings(&objects[jj], 1, &s_section);   
            UF_CALL(UF_MODL_create_bplane(&s_section, tol, &bplane));   
            UF_MODL_free_string_list(&s_section);   
        }   
        UF_free(objects);   
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

> 这段代码是用于NX的二次开发代码，其功能是从NX中的封闭单线样条曲线创建有界平面。以下是代码的主要功能：
>
> 1. 定义了错误报告函数report_error，用于输出错误信息。
> 2. 定义了内存分配函数allocate_memory，用于分配内存。
> 3. 定义了创建数组函数make_an_array，用于将列表转换为数组。
> 4. 定义了查询所有曲线函数ask_all_of_type，用于查询零件中的所有样条曲线。
> 5. 定义了查询曲线端点函数ask_curve_ends，用于查询样条曲线的起点和终点。
> 6. 定义了排序函数sort_into_strings，用于将曲线按顺序连接成字符串。
> 7. 定义了主函数do_it，用于遍历所有曲线，将它们连接成字符串，然后创建有界平面。
> 8. 定义了ufusr函数，是NX调用的入口函数，调用do_it函数执行主要功能。
> 9. 定义了卸载函数ufusr_ask_unload，用于卸载NX中的用户函数。
>
> 总的来说，这段代码实现了从NX中的封闭单线样条曲线创建有界平面的功能，通过查询曲线，排序连接，最后创建有界平面。
>
