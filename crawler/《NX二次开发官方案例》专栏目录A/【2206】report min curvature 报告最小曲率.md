### 【2206】report min curvature 报告最小曲率

#### 代码

```cpp
    /*HEAD REPORT_MIN_CURVATURE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <math.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_eval.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
    #include <uf_undo.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是在V18版本中新增的。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
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
    static tag_t select_a_curve(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static double ask_arc_radius(tag_t arc)  
    {  
        double  
            radius;  
        UF_EVAL_p_t  
            evaluator;  
        UF_EVAL_arc_t  
            e_arc;  
        UF_CALL(UF_EVAL_initialize(arc, &evaluator));  
        UF_CALL(UF_EVAL_ask_arc(evaluator, &e_arc));  
        radius = e_arc.radius;  
        UF_CALL(UF_EVAL_free(evaluator));  
        return radius;  
    }  
    static void ask_arc_center(tag_t arc, double center[3])  
    {  
        UF_EVAL_p_t  
            evaluator;  
        UF_EVAL_arc_t  
            e_arc;  
        UF_CALL(UF_EVAL_initialize(arc, &evaluator));  
        UF_CALL(UF_EVAL_ask_arc(evaluator, &e_arc));  
        UF_VEC3_copy(e_arc.center, center);  
        UF_CALL(UF_EVAL_free(evaluator));  
    }  
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n,  
            subtype,  
            type;  
        tag_t  
            curve,  
            *segs;  
        double  
            min,  
            pos[3],  
            radius,  
            tol;  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_MODL_ask_distance_tolerance(&tol);  
        while ((curve = select_a_curve("Select a curve")) != NULL_TAG)  
        {  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL_TAG, &mark_id));  
            UF_CALL(UF_OBJ_ask_type_and_subtype(curve, &type, &subtype));  
            if (type == UF_solid_type)  
                UF_CALL(UF_MODL_create_curve_from_edge(curve, &curve));  
            UF_CALL(UF_CURVE_create_simplified_curve(1, &curve, tol, &n, &segs));  
            min = HUGE_VAL;  
            for (ii = 0; ii < n; ii++)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(segs[ii], &type, &subtype));  
                if (type == UF_circle_type)  
                {  
                    radius = ask_arc_radius(segs[ii]);  
                    if (radius < min)  
                    {  
                        min = radius;  
                        ask_arc_center(segs[ii], pos);  
                    }  
                }  
            }  
            UF_free(segs);  
            UF_CALL(UF_UNDO_undo_to_mark(mark_id, NULL_TAG));  
            if (min < HUGE_VAL)  
            {  
                WRITE_F(min);  
                display_temporary_point(pos);  
            }  
            else  
            {  
                WRITE("that curve is straight\n");  
            }  
        }  
    }  
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

> 根据代码内容，这是用于NX软件的二次开发代码，主要功能是选择曲线并找出其最小曲率半径。以下是代码的主要功能介绍：
>
> 1. 定义错误报告函数report_error，用于打印错误信息。
> 2. 定义WRITE和WRITE_F宏，用于向列表窗口打印信息。
> 3. 定义mask_for_curves函数，用于设置选择过滤条件，只允许选择线、圆、圆锥曲线、样条线。
> 4. 定义select_a_curve函数，用于选择一条曲线，并高亮显示。
> 5. 定义ask_arc_radius和ask_arc_center函数，用于查询圆弧的半径和圆心。
> 6. 定义display_temporary_point函数，用于在指定位置显示一个临时点。
> 7. 定义do_it函数，用于执行主要流程：不断循环选择曲线创建简化曲线遍历简化曲线中的所有圆弧，找出最小半径和对应圆心如果找到最小半径，打印并显示圆心，否则打印直线的提示撤销操作回到选择前状态
> 8. 不断循环选择曲线
> 9. 创建简化曲线
> 10. 遍历简化曲线中的所有圆弧，找出最小半径和对应圆心
> 11. 如果找到最小半径，打印并显示圆心，否则打印直线的提示
> 12. 撤销操作回到选择前状态
> 13. 定义ufusr函数，作为二次开发程序的入口函数。
> 14. 定义ufusr_ask_unload函数，用于设置程序卸载模式。
>
> 总体来说，这段代码实现了选择曲线并找出最小曲率半径的功能。
>
