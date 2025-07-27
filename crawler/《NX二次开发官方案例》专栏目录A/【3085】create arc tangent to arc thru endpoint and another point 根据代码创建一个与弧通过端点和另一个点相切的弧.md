### 【3085】create arc tangent to arc thru endpoint and another point 根据代码创建一个与弧通过端点和另一个点相切的弧

#### 代码

```cpp
    /*HEAD CREATE_ARC_TANGENT_TO_ARC_THRU_ENDPOINT_AND_ANOTHER_POINT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <math.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_eval.h>  
    #include <uf_curve.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
    #include <uf_vec.h>  
    #include <uf_trns.h>  
    #include <uf_modl.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译如下：

注意：UF_print_syslog在V18中是新增的，只需回答翻译，不要添加无关内容。 */  
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
    static void map_abs2view(double c[3])  
    {  
        char  
            vname[30];  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(abs_mx, vw, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void map_view2abs(double c[3])  
    {  
        char  
            vname[30];  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(vw, abs_mx, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void ask_pos_on_obj(tag_t obj, double loc[3])  
    {  
        tag_t  
            line;  
        double  
            cp[3],  
            dist;  
        UF_CURVE_line_t  
            lp;  
        map_abs2view(loc);  
        lp.start_point[0] = loc[0];  
        lp.start_point[1] = loc[1];  
        lp.start_point[2] = loc[2] + 1000;  
        lp.end_point[0] = loc[0];  
        lp.end_point[1] = loc[1];  
        lp.end_point[2] = loc[2] - 1000;  
        map_view2abs(lp.start_point);  
        map_view2abs(lp.end_point);  
        UF_CALL(UF_CURVE_create_line(&lp, &line));  
        UF_CALL(UF_MODL_ask_minimum_dist(obj, line, 0, cp, 0, cp, &dist, loc, cp));  
        UF_CALL(UF_OBJ_delete_object(line));  
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
    static void ask_curve_close_endpoint(tag_t curve, double pos[3])  
    {  
        double  
            dist[2],  
            ends[6];  
        ask_curve_ends(curve, &ends[0], &ends[3]);  
        UF_VEC3_distance(pos, ends, &dist[0]);  
        UF_VEC3_distance(pos, &ends[3], &dist[1]);  
        if (dist[0] < dist[1]) UF_VEC3_copy(ends, pos);  
        else UF_VEC3_copy(&ends[3], pos);  
    }  
    static void ask_edge_close_endpoint(tag_t edge, double pos[3])  
    {  
        int  
            cnt;  
        double  
            parm[3],  
            end[3],  
            start[3];  
        UF_CALL(UF_MODL_ask_edge_verts(edge, start, end, &cnt));  
        UF_CALL(UF_MODL_create_curve_from_edge(edge, &edge));  
        UF_CALL(UF_MODL_ask_curve_parm(edge, start, &parm[0], start));  
        UF_CALL(UF_MODL_ask_curve_parm(edge, pos, &parm[1], pos));  
        UF_CALL(UF_MODL_ask_curve_parm(edge, end, &parm[2], end));  
        UF_CALL(UF_OBJ_delete_object(edge));  
        if (fabs(parm[1] - parm[0]) < fabs(parm[2] - parm[1]))  
             UF_VEC3_copy(start, pos);  
        else UF_VEC3_copy(end, pos);  
    }  
    /* 里海 */  
    static int mask_for_arcs(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_circle_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_CIRCULAR_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_an_arc_endpoint(char *prompt, double pos[3])  
    {  
        int  
            subtype,  
            resp,  
            type;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select arc endpoint", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_arcs, NULL, &resp,  
            &object, pos, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            ask_pos_on_obj(object, pos);  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            if (type == UF_solid_type) ask_edge_close_endpoint(object, pos);  
            else ask_curve_close_endpoint(object, pos);  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* 里海 */  
    static int mask_for_points(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_point_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_point(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a point", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_points, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            arc_opts[1] = { UF_CURVE_FILLET_IN_CIRCLE },  
            no_trim[2] = { FALSE, FALSE };  
        tag_t  
            arc,  
            obj[3];  
        double  
            angle,  
            binormal[3],  
            center[3],  
            chord_len,  
            chord_vec[3],  
            ccw[3],  
            parm,  
            pos[3],  
            normal[3],  
            radius,  
            tangent[3],  
            thru_pos[3],  
            to_center[3];  
        UF_EVAL_p_t  
            eval;  
        while (((obj[1] = select_an_arc_endpoint("Select tangent arc", pos))  
                    != NULL_TAG) &&  
                ((obj[0] = select_a_point("Select thru point")) != NULL_TAG))  
        {  
            UF_CALL(UF_EVAL_initialize(obj[1], &eval));  
            UF_CALL(UF_EVAL_evaluate_closest_point(eval, pos, &parm, pos));  
            UF_CALL(UF_EVAL_evaluate_unit_vectors(eval, parm, pos, tangent, normal,  
                binormal));  
            UF_CALL(UF_CURVE_ask_point_data(obj[0], thru_pos));  
            UF_VEC3_sub(thru_pos, pos, chord_vec);  
            UF_VEC3_distance(pos, thru_pos, &chord_len);  
            UF_VEC3_cross(normal, chord_vec, ccw);  
            UF_VEC3_angle_between(normal, chord_vec, ccw, &angle);  
            radius = (chord_len/2) / cos (angle);  
            UF_VEC3_scale( radius, normal, to_center );  
            UF_VEC3_add(pos, to_center, center);  
            UF_CALL(UF_CURVE_create_fillet(UF_CURVE_2_CURVE, obj, center, radius,  
                no_trim, arc_opts,  &arc));  
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

> 这段代码是一个NX二次开发程序，其主要功能是创建一个切线圆弧连接两个圆弧的端点和一个通过点。
>
> 主要功能如下：
>
> 1. 定义了一些辅助函数，包括错误报告函数report_error，视图坐标转换函数map_abs2view和map_view2abs，获取对象上的点函数ask_pos_on_obj，获取曲线端点函数ask_curve_ends和ask_curve_close_endpoint，获取边缘端点函数ask_edge_close_endpoint等。
> 2. 定义了两个选择集掩码，用于选择圆弧端点和点。
> 3. do_it函数是主函数，它首先选择一个圆弧端点和一个点，然后根据这两个点创建一个切线圆弧。具体步骤包括：初始化圆弧端点的评估器，并获取切线方向和法线方向。获取通过点的坐标。计算弦向量，并计算圆弧的半径和中心。调用UF_CURVE_create_fillet函数创建切线圆弧。
> 4. 初始化圆弧端点的评估器，并获取切线方向和法线方向。
> 5. 获取通过点的坐标。
> 6. 计算弦向量，并计算圆弧的半径和中心。
> 7. 调用UF_CURVE_create_fillet函数创建切线圆弧。
> 8. ufusr函数是NX调用的主函数，用于初始化和调用do_it函数。
> 9. ufusr_ask_unload函数用于卸载用户函数。
>
> 总的来说，这段代码实现了一个简单的创建切线圆弧的功能，通过选择圆弧端点和点，并调用NX提供的曲线创建函数来实现。
>
