### 【0742】create joined curves from selected faces loops 创建选定面环的连接曲线

#### 代码

```cpp
    /*HEAD CREATE_JOINED_CURVES_FROM_SELECTED_FACES_LOOPS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_eval.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog在V18中是新的。 */  
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
    static int mask_for_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void display_temporary_colored_point(double *coords, int color)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        attrib.color = color;  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static void display_temporary_text(char *text, double loc[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static void ask_curve_ends(tag_t curve, double start[3], double end[3])  
    {  
        double  
            limits[2];  
        UF_EVAL_p_t  
            evaluator;  
        UF_CALL(UF_EVAL_initialize(curve, &evaluator));  
        UF_CALL(UF_EVAL_ask_limits(evaluator, limits));  
        UF_CALL(UF_EVAL_evaluate(evaluator, 0, limits[0], start, NULL));  
        UF_CALL(UF_EVAL_evaluate(evaluator, 0, limits[1], end, NULL));  
        UF_CALL(UF_EVAL_free(evaluator));  
    }  
    static double mark_gaps_in_curve_list(uf_list_p_t objects)  
    {  
        int  
            ii = 0,  
            small;  
        double  
            biggest_gap = 0.0,  
            distance_tolerance,  
            gap[4],  
            midpt[4][3],  
            last_start[3],  
            last_end[3],  
            this_start[3],  
            this_end[3];  
        char  
            msg[133];  
        uf_list_p_t  
            tmp;  
        UF_MODL_ask_distance_tolerance(&distance_tolerance);  
        for (tmp = objects; tmp != NULL; tmp = tmp->next)  
        {  
            if (tmp != objects)  
            {  
                ask_curve_ends(tmp->eid, this_start, this_end);  
                UF_VEC3_distance(last_start, this_start, &gap[0]);  
                UF_VEC3_midpt(last_start, this_start, midpt[0]);  
                UF_VEC3_distance(last_end, this_start, &gap[1]);  
                UF_VEC3_midpt(last_end, this_start, midpt[1]);  
                UF_VEC3_distance(last_start, this_end, &gap[2]);  
                UF_VEC3_midpt(last_start, this_end, midpt[2]);  
                UF_VEC3_distance(last_end, this_end, &gap[3]);  
                UF_VEC3_midpt(last_end, this_end, midpt[3]);  
                for (ii = 0; ii < 4; ii++)  
                {  
                    if (ii == 0) small = 0;  
                    else if (gap[ii] < gap[small]) small = ii;  
                    if (gap[ii] < distance_tolerance) break;  
                }  
                if (gap[small] > biggest_gap) biggest_gap = gap[small];  
                if (ii == 4)  
                {  
                    sprintf(msg, "gap = %f", gap[small]);  
                    display_temporary_text(msg, midpt[small]);  
                    display_temporary_colored_point(last_start, UF_OBJ_BLUE);  
                    display_temporary_colored_point(last_end, UF_OBJ_BLUE);  
                    display_temporary_colored_point(this_start, UF_OBJ_GREEN);  
                    display_temporary_colored_point(this_end, UF_OBJ_GREEN);  
                }  
            }  
            ask_curve_ends(tmp->eid, last_start, last_end);  
        }  
        return biggest_gap;  
    }  
    static void do_it(void)  
    {  
        int  
            err,  
            has_corners,  
            n_edges;  
        double  
            old_distol,  
            max_gap;  
        tag_t  
            face,  
            joined_curve;  
        uf_loop_p_t  
            loop_list,  
            tmp;  
        while ((face = select_a_face("Join edge loops")) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_face_loops(face, &loop_list));  
            if (loop_list != NULL)  
            {  
                for (tmp = loop_list; tmp != NULL; tmp = tmp->next)  
                {  
                    UF_CALL(UF_MODL_ask_list_count(tmp->edge_list, &n_edges));  
                    if (n_edges > 1)  
                    {  
                        if (UF_CURVE_create_joined_curve(tmp->edge_list,  
                            1, &joined_curve, &has_corners))  
                        {  
                            max_gap = mark_gaps_in_curve_list(tmp->edge_list);  
                            UF_MODL_ask_distance_tolerance(&old_distol);  
                            UF_MODL_set_distance_tolerance(&max_gap, &err);  
                            UF_CALL(UF_CURVE_create_joined_curve(tmp->edge_list,  
                                1, &joined_curve, &has_corners));  
                            UF_MODL_set_distance_tolerance(&old_distol, &err);  
                        }  
                    }  
                }  
                UF_CALL(UF_MODL_delete_loop_list(&loop_list));  
            }  
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

> 这段代码是用于NX的二次开发，主要实现了以下功能：
>
> 1. 创建一个对话框，允许用户选择一个面，并获取该面的边环。
> 2. 对于每个边环，如果它包含多条边，则将这些边连接成一条曲线。
> 3. 计算曲线之间的间隙，并显示最大间隙。
> 4. 如果最大间隙大于NX的公差设置，则调整公差，重新连接曲线，直到间隙小于公差。
> 5. 重复以上步骤，直到用户没有选择新的面。
>
> 主要函数包括：
>
> 通过这个代码，用户可以方便地连接所选面的边环，并自动调整公差，以避免间隙过大。
>
