### 【2903】sort selected curves into strings 将选定的曲线排序成字符串

#### 代码

```cpp
    /*HEAD SORT_SELECTED_CURVES_INTO_STRINGS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：
注意：UF_print_syslog在V18版本中是新增的。请只提供翻译，不要添加任何无关的内容。 */  
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
            mask[5] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, UF_all_subtype, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_point_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 5, mask)))  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_SL(X) report_string_list_info(#X, X)  
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
    static void display_temporary_text(char *text, double loc[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static void report_string_list_info(char *name, UF_STRING_t *string)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        double  
            loc[3];  
        char  
            msg[133];  
        sprintf(msg, "%s->num = %d\n", name, string->num);  
        WRITE(msg);  
        for (ii = 0; ii < string->num; ii++)  
        {  
            sprintf(msg, "%s->string[%d] = %d\n", name, ii, string->string[ii]);  
            WRITE(msg);  
            sprintf(msg, "%s->dir[%d] = %d\n", name, ii, string->dir[ii]);  
            WRITE(msg);  
            if (string->dir[ii] == UF_MODL_CURVE_START_FROM_BEGIN)  
                show_curve_direction(string->id[kk]);  
            else  
                show_curve_flip_direction(string->id[kk]);  
            for (jj = 0; jj < string->string[ii]; jj++)  
            {  
                sprintf(msg, "%d", kk);  
                if (UF_OBJ_ask_name_origin(string->id[kk], loc))  
                {  
                    UF_CALL(UF_OBJ_set_name(string->id[kk], "X"));  
                    UF_CALL(UF_OBJ_ask_name_origin(string->id[kk], loc));  
                    UF_CALL(UF_OBJ_delete_name(string->id[kk]));  
                }  
                display_temporary_text(msg, loc);  
                sprintf(msg, "%s->id[%d] = %d\n", name, kk, string->id[kk]);  
                WRITE(msg);  
                UF_CALL(UF_DISP_set_highlight(string->id[kk], TRUE));  
                kk++;  
            }  
            sprintf(msg, "%d string curves highlighted", string->string[ii]);  
            uc1601(msg, TRUE);  
            UF_CALL(UF_PART_cleanup(UF_PART_cleanup_highlight));  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            *curves;  
        UF_STRING_t  
            string;  
        while ((n = select_curves("Select curves to sort", &curves)) > 0)  
        {  
            UF_DISP_refresh();  
            sort_into_strings(curves, n, &string);  
            WRITE_SL(&string);  
            UF_MODL_free_string_list(&string);  
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

> 这段NX二次开发代码的主要功能是将用户选择的多条曲线按照连接关系排序成多个字符串。具体介绍如下：
>
> 1. 功能模块划分：代码包括错误处理模块、曲线选择模块、字符串排序模块、字符串信息报告模块和主函数模块。
> 2. 错误处理模块：该模块包含自定义的错误处理函数report_error，用于在函数调用出错时输出错误信息。
> 3. 曲线选择模块：该模块包含自定义的曲线选择函数select_curves，利用UF_UI库提供的对话框让用户选择曲线，并返回选择的曲线数组。
> 4. 字符串排序模块：该模块包含自定义的字符串排序函数sort_into_strings，用于根据曲线端点距离将选择的曲线排序成多个字符串，并保存到UF_STRING结构中。
> 5. 字符串信息报告模块：该模块包含自定义的字符串信息报告函数report_string_list_info，用于输出每个字符串包含的曲线数量、方向等信息，并在图形区显示。
> 6. 主函数模块：该模块包含主函数do_it，用于循环调用曲线选择和字符串排序，并在每个循环中调用字符串信息报告函数。
> 7. UFusr函数：该函数是NX二次开发的入口函数，用于初始化和调用主函数。
> 8. 卸载函数：该函数用于在卸载用户自定义程序时调用。
>
> 综合来看，这段代码实现了根据曲线连接关系排序成字符串的功能，并提供了良好的交互和错误处理。
>
