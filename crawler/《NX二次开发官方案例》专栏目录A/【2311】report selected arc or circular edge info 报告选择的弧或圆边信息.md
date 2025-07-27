### 【2311】report selected arc or circular edge info 报告选择的弧或圆边信息

#### 代码

```cpp
    锘/*HEAD REPORT_SELECTED_ARC_OR_CIRCULAR_EDGE_INFO CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_csys.h>  
    #include <uf_eval.h>  
    #include <uf_vec.h>  
    #include <stdarg.h>  
    static void ECHO(char* format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN + 1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error(char* file, int line, char* call, int irc)  
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
    static int mask_for_arcs(UF_UI_selection_p_t select, void* type)  
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
    static tag_t select_an_arc(char* prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an arc", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_arcs, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #define WRITE_N(X) ECHO("%s = %d\n", #X, X)  
    #define WRITE_F(X) ECHO("%s = %f\n", #X, X)  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    static void write_double_array_to_listing_window(char* name, double* array,  
        int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN + 1];  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (cols > 0)  
                if (ii == 0) ECHO("%s = ", name);  
                else ECHO("%s", blanks);  
            for (jj = 0; jj < cols; jj++)  
            {  
                ECHO("%f", array[kk++]);  
                if (jj < cols - 1) ECHO("%s", ", ");  
            }  
            if (cols > 0) ECHO("%s", "\n");  
        }  
    }  
    static void display_temporary_point(double* coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE };  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static void report_arc_info(tag_t arc)  
    {  
        tag_t  
            display_part = UF_PART_ask_display_part(),  
            work_part = UF_ASSEM_ask_work_part();  
        double  
            abs_offset,  
            abs_x[3] = { 1, 0, 0 },  
            abs_z[3] = { 0, 0, 1 },  
            arc_length,  
            abs_end_angle,  
            end_disp_abs[3],  
            end_disp_wcs[3],  
            end_work_abs[3],  
            center_disp_wcs[3],  
            center_work_abs[3],  
            delta_angle,  
            diameter,  
            abs_start_angle,  
            start_disp_abs[3],  
            start_disp_wcs[3],  
            start_work_abs[3];  
        UF_EVAL_p_t  
            evaluator;  
        UF_EVAL_arc_t  
            e_arc;  
        logical  
            is_arc;  
        UF_CALL(UF_EVAL_initialize(arc, &evaluator));  
        UF_CALL(UF_EVAL_is_arc(evaluator, &is_arc));  
        WRITE_N(is_arc);  
        UF_CALL(UF_EVAL_ask_arc(evaluator, &e_arc));  
        UF_CALL(UF_EVAL_evaluate(evaluator, 0, e_arc.limits[0], start_disp_abs,  
            NULL));  
        UF_CALL(UF_EVAL_evaluate(evaluator, 0, e_arc.limits[1], end_disp_abs,  
            NULL));  
        UF_CALL(UF_EVAL_free(evaluator));  
        WRITE_F(e_arc.limits[0] * RADEG);  
        WRITE_F(e_arc.limits[1] * RADEG);  
        UF_VEC3_angle_between(abs_x, e_arc.x_axis, abs_z, &abs_offset);  
        abs_start_angle = e_arc.limits[0] + abs_offset;  
        if (abs_start_angle > (2 * PI)) abs_start_angle = abs_start_angle - (2 * PI);  
        WRITE_F(abs_start_angle * RADEG);  
        abs_end_angle = e_arc.limits[1] + abs_offset;  
        if (abs_end_angle > (2 * PI)) abs_end_angle = abs_end_angle - (2 * PI);  
        WRITE_F(abs_end_angle * RADEG);  
        UF_DISP_labeled_conehead(UF_DISP_ALL_ACTIVE_VIEWS, e_arc.center,  
            e_arc.x_axis, 0, "X");  
        UF_DISP_labeled_conehead(UF_DISP_ALL_ACTIVE_VIEWS, e_arc.center,  
            e_arc.y_axis, 0, "Y");  
        display_temporary_point(e_arc.center);  
        WRITE_F(e_arc.radius);  
        WRITE_F(diameter = e_arc.radius * 2);  
        WRITE_F(arc_length = (e_arc.limits[1] - e_arc.limits[0]) * e_arc.radius);  
        WRITE_F(delta_angle = (e_arc.limits[1] - e_arc.limits[0]) * RADEG);  
        ECHO("\n");  
        WRITE3F(e_arc.center);  
        WRITE3F(start_disp_abs);  
        WRITE3F(end_disp_abs);  
        ECHO("\n");  
        UF_CALL(UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, e_arc.center,  
            UF_CSYS_ROOT_WCS_COORDS, center_disp_wcs));  
        UF_CALL(UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, start_disp_abs,  
            UF_CSYS_ROOT_WCS_COORDS, start_disp_wcs));  
        UF_CALL(UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, end_disp_abs,  
            UF_CSYS_ROOT_WCS_COORDS, end_disp_wcs));  
        WRITE3F(center_disp_wcs);  
        WRITE3F(start_disp_wcs);  
        WRITE3F(end_disp_wcs);  
        ECHO("\n");  
        if (work_part != display_part)  
        {  
            UF_CALL(UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, e_arc.center,  
                UF_CSYS_WORK_COORDS, center_work_abs));  
            UF_CALL(UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, start_disp_abs,  
                UF_CSYS_WORK_COORDS, start_work_abs));  
            UF_CALL(UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, end_disp_abs,  
                UF_CSYS_WORK_COORDS, end_work_abs));  
            WRITE3F(center_work_abs);  
            WRITE3F(start_work_abs);  
            WRITE3F(end_work_abs);  
            ECHO("\n");  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            arc;  
        while ((arc = select_an_arc("Report arc info")) != NULL_TAG)  
            report_arc_info(arc);  
    }  
    /* qq3123197280 */  
    void ufusr(char* param, int* retcode, int paramLen)  
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

> 这段代码是一个NX Open API的二次开发示例，主要功能是选择圆弧并报告其信息。代码的主要部分包括：
>
> 1. ECHO宏：用于在列表窗口和系统日志中输出信息。
> 2. UF_CALL宏：用于封装UF函数调用，并报告错误。
> 3. mask_for_arcs函数：用于设置选择掩码，只选择圆弧和圆形边线。
> 4. select_an_arc函数：使用对话框选择一个圆弧，并高亮显示。
> 5. WRITE_XXX宏：用于输出变量信息。
> 6. display_temporary_point函数：用于显示一个临时点。
> 7. report_arc_info函数：报告选择的圆弧信息，包括是否为圆弧、圆心坐标、半径、直径、弧长、起始角度、终止角度、X轴、Y轴等。
> 8. do_it函数：循环选择圆弧并报告其信息。
> 9. ufusr函数：初始化和终止NX Open API。
> 10. ufusr_ask_unload函数：卸载用户程序。
>
> 总体来说，这段代码通过选择圆弧并调用UF_EVAL函数获取其详细信息，实现了圆弧信息的查询功能。
>
