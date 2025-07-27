### 【2461】reverse arc direction 反转弧线方向

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_object_types.h>  
    #include <uf_csys.h>  
    #include <uf_vec.h>  
    #include <uf_trns.h>  
    #include <uf_modl.h>  
    #include <uf_eval.h>  
    #include <uf_obj.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    /* qq3123197280 */  
    static int mask_for_arcs(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[1] = { { UF_circle_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_an_arc(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an arc", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_arcs, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #include <uf_undo.h>  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void map_matrix2matrix(double *ref_mx, double *dest_mx, double loc[3])  
    {  
        int  
            ii,  
            irc;  
        double  
            d_csys[12] = { 0,0,0, 0,0,0, 0,0,0, 0,0,0 },  
            mx[16],  
            r_csys[12] = { 0,0,0, 0,0,0, 0,0,0, 0,0,0 };  
        for (ii = 0; ii < 9; ii++)  
        {  
            r_csys[ii+3] = ref_mx[ii];  
            d_csys[ii+3] = dest_mx[ii];  
        }  
        FTN(uf5940)(r_csys, d_csys, mx, &irc);  
        FTN(uf5941)(loc, mx);  
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
    #define WRITE_F(X) ECHO("%s = %f\n", #X, X)  
    static void do_it(void)  
    {  
        tag_t  
            arc;  
        double  
            orig_start[3],  
            orig_end[3],  
            parm,  
            flip_csys[9],  
            orig_csys[9];  
        UF_CURVE_arc_t  
            orig_data,  
            flip_data;  
        while ((arc = select_an_arc("Reverse Direction")) != NULL_TAG)  
        {  
            show_curve_direction(arc);  
            ask_curve_ends(arc, orig_start, orig_end);  
            set_undo_mark("Reverse Arc Direction");  
            UF_CALL(UF_CURVE_ask_arc_data(arc, &orig_data));  
            UF_CALL(UF_CURVE_ask_arc_data(arc, &flip_data));  
            UF_CALL(UF_CSYS_ask_matrix_values(orig_data.matrix_tag, orig_csys));  
            UF_VEC3_copy(&(orig_csys[0]), &(flip_csys[0]));  
            UF_VEC3_negate(&(orig_csys[3]), &(flip_csys[3]));  
            UF_VEC3_negate(&(orig_csys[6]), &(flip_csys[6]));  
            UF_CALL(UF_CSYS_create_matrix(flip_csys, &flip_data.matrix_tag));  
            UF_VEC3_copy(&(orig_data.arc_center[0]), &(flip_data.arc_center[0]));  
            map_matrix2matrix(orig_csys, flip_csys, flip_data.arc_center);  
            flip_data.start_angle = 0;  
            flip_data.end_angle = TWOPI;  
            UF_CALL(UF_CURVE_edit_arc_data(arc, &flip_data));  
            UF_CALL(UF_MODL_ask_curve_parm(arc, orig_end, &parm, orig_end));  
            flip_data.start_angle = parm * TWOPI;  
            UF_CALL(UF_MODL_ask_curve_parm(arc, orig_start, &parm, orig_start));  
            flip_data.end_angle = parm * TWOPI;  
            if (flip_data.start_angle >= flip_data.end_angle)  
                flip_data.end_angle = flip_data.end_angle + TWOPI;  
            UF_CALL(UF_CURVE_edit_arc_data(arc, &flip_data));  
            show_curve_direction(arc);  
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

> 这是段用于NX 8.5的二次开发代码，主要实现了选择一个弧，然后反转其方向的逻辑。以下是该代码的主要功能：
>
> 1. 错误处理和日志输出：通过report_error函数来记录错误信息，并通过ECHO函数输出日志。
> 2. 弧选择：定义了一个弧选择函数select_an_arc，使用对话框让用户选择一个弧，并进行高亮显示。
> 3. 弧方向显示：使用show_curve_direction函数在弧的起点处显示一个红色圆锥头，以指示弧的方向。
> 4. 弧数据获取和编辑：通过ask_curve_ends函数获取弧的起点和终点坐标，然后通过UF_CURVE_ask_arc_data和UF_CURVE_edit_arc_data函数获取和修改弧的数据。
> 5. 反转弧方向：通过反转弧的坐标系和角度来反转弧的方向。
> 6. 撤销标记：使用set_undo_mark函数在反转弧方向前后设置撤销标记点，以便撤销操作。
> 7. 主函数：do_it函数实现了选择弧、反转方向、显示新方向的流程。
> 8. 用户子程序入口：ufusr函数是用户子程序的入口点，用于初始化和调用do_it函数。
> 9. 卸载函数：ufusr_ask_unload函数声明了该程序的卸载策略。
>
> 综上所述，这段代码通过二次开发实现了选择并反转弧方向的功能，同时包含了错误处理和日志输出等辅助功能。
>
