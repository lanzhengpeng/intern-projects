### 【2857】show all helix origins and directions 显示所有螺旋的起点和方向

#### 代码

```cpp
    /*HEAD SHOW_ALL_HELIX_ORIGINS_AND_DIRECTIONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_eval.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
    #include <uf_vec.h>  
    #include <uf_undo.h>  
    #include <uf_part.h>  
    #include <uf_so.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的描述，UF_print_syslog 是在 V18 版本中新增的函数。在文档中提到，只回答翻译内容，不要添加任何废话。因此，我的回答是：

UF_print_syslog 是 V18 版本新增的函数。 */  
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
    static tag_t ask_next_feature_of_type(tag_t part, char *type, tag_t feat)  
    {  
        char  
            *this_type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
        {  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE)  
            {  
                UF_CALL(UF_MODL_ask_feat_type(feat, &this_type));  
                if (!strcmp(this_type, type))  
                {  
                    UF_free(this_type);  
                    return feat;  
                }  
                UF_free(this_type);  
            }  
        }  
        return NULL_TAG;  
    }  
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static void display_conehead(double origin[3], double vec[3])  
    {  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);  
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
    static void ask_curve_midpoint(tag_t curve, double midpoint[3])  
    {  
        tag_t  
            last,  
            offset,  
            start;  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark_id));  
        UF_CALL(UF_SO_create_scalar_double(curve, UF_SO_update_after_modeling,  
            0.0, &start));  
        UF_CALL(UF_SO_create_point_on_curve(curve, UF_SO_update_after_modeling,  
            curve, start, &last));  
        UF_CALL(UF_SO_set_visibility_option(last, UF_SO_invisible));  
        UF_CALL(UF_SO_create_scalar_double(curve, UF_SO_update_after_modeling,  
            0.5, &offset));  
        UF_CALL(UF_SO_create_point_along_curve(curve, UF_SO_update_after_modeling,  
            curve, last, offset, UF_SO_point_along_curve_percent, FALSE, &last));  
        UF_CALL(UF_CURVE_ask_point_data(last, midpoint));  
        UF_CALL(UF_UNDO_undo_to_mark(mark_id, NULL));  
        UF_CALL(UF_UNDO_delete_mark(mark_id, NULL));  
    }  
    static logical ask_helix_origin_and_direction(tag_t helix_feature,  
        double origin[3], double direction[3])  
    {  
        int  
            ii,  
            n_curves,  
            n_exps,  
            whole_turns;  
        tag_t  
            *curves,  
            *exps,  
            plane,  
            point,  
            radius_exp = NULL_TAG,  
            turns_exp = NULL_TAG;  
        double  
            center[3],  
            dist,  
            end[3],  
            junk[3],  
            midpoint[3],  
            n_turns,  
            start[3];  
        char  
            **descs,  
            *lhs,  
            new_exp[UF_MAX_EXP_LENGTH+1],  
            *rhs,  
            *string;  
        UF_UNDO_mark_id_t  
            mark_id;  
        if (UF_CALL(UF_MODL_ask_exp_desc_of_feat(helix_feature, &n_exps, &descs,  
            &exps))) return FALSE;  
        for (ii = 0; ii < n_exps; ii++)  
        {  
            if (!strcmp(descs[ii], "Radius")) radius_exp = exps[ii];  
            if (!strcmp(descs[ii], "Number of Turns")) turns_exp = exps[ii];  
        }  
        if (n_exps > 0)  
        {  
            UF_free(exps);  
            UF_free_string_array(n_exps, descs);  
        }  
        if ((radius_exp == NULL_TAG) || (turns_exp == NULL_TAG)) return FALSE;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &mark_id));  
        UF_CALL(UF_MODL_ask_exp_tag_value(turns_exp, &n_turns));  
        whole_turns = (int) n_turns;  
        if (n_turns != whole_turns)  
        {  
            if (whole_turns < 1) whole_turns = 1;  
            UF_CALL(UF_MODL_ask_exp_tag_string(turns_exp, &string));  
            UF_CALL(UF_MODL_dissect_exp_string(string, &lhs, &rhs,  
                &turns_exp));  
            sprintf(new_exp, "%s=%d", lhs, whole_turns);  
            UF_CALL(UF_MODL_edit_exp(new_exp));  
            UF_CALL(UF_MODL_update());  
            UF_free(string);  
            UF_free(lhs);  
            UF_free(rhs);  
        }  
        UF_CALL(UF_MODL_ask_feat_object(helix_feature, &n_curves, &curves));  
        ask_curve_ends(curves[0], start, end);  
        ask_curve_midpoint(curves[0], midpoint);  
        UF_free(curves);  
        UF_VEC3_sub(end, start, direction);  
        UF_VEC3_midpt(start, midpoint, center);  
        UF_CALL(UF_CURVE_create_point(center, &point));  
        UF_CALL(UF_MODL_create_plane(start, direction, &plane));  
        UF_CALL(UF_MODL_ask_minimum_dist(plane, point, FALSE, NULL, FALSE,  
            NULL, &dist, origin, junk));  
        UF_CALL(UF_UNDO_undo_to_mark(mark_id, NULL));  
        UF_CALL(UF_UNDO_delete_mark(mark_id, NULL));  
        return TRUE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            helix = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        double  
            origin[3],  
            z_axis[3];  
        char  
            *name;  
        while ((helix = ask_next_feature_of_type(part,"HELIX",helix)) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_feat_name(helix, &name));  
            if (ask_helix_origin_and_direction(helix, origin, z_axis))  
            {  
                display_temporary_point(origin);  
                display_conehead(origin, z_axis);  
                UF_DISP_set_highlight(helix, TRUE);  
                uc1601(name, TRUE);  
                UF_DISP_set_highlight(helix, FALSE);  
            }  
            else  
            {  
                UF_UI_open_listing_window();  
                UF_UI_write_listing_window(name);  
                UF_UI_write_listing_window(": Cannot determine origin/direction\n");  
            }  
            UF_free(name);  
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

> 这段NX二次开发代码的主要功能是遍历当前零件中的所有螺旋特征，并显示每个螺旋特征的起源和方向。具体来说，代码首先初始化NX环境，然后通过遍历零件中的特征，找到所有类型为“HELIX”的螺旋特征。对于每个螺旋特征，代码会提取螺旋的起始点、终止点和中心点，然后计算螺旋的方向向量。最后，代码会在图形窗口中显示螺旋的起源和方向，并高亮显示螺旋特征。整个功能通过定义多个辅助函数来实现，包括提取螺旋特征的参数表达式、计算螺旋的起始点和终止点、创建平面和点等。代码结构清晰，注释详细，易于理解。
>
