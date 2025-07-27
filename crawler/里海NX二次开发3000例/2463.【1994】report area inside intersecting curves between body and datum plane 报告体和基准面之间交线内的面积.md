### 【1994】report area inside intersecting curves between body and datum plane 报告体和基准面之间交线内的面积

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_curve.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_text.h>  
    #include <uf_ui.h>  
    #include <uf_undo.h>  
    #include <uf_vec.h>  
    #include <stdarg.h>  
    #pragma warning(disable:4996)  
    static void ECHO(char *format, ...)  
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
    static int report_error(char *file, int line, char *call, int irc)  
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
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_body(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a body", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static int mask_for_datum_planes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_datum_plane_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
            UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_datum_plane(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a datum plane", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_datum_planes, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void display_temporary_colored_text(char *text, double loc[3], int color)  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
            UF_OBJ_FONT_SOLID, FALSE };  
        props.color = color;  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_ACTIVE_MINUS,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static void ask_curve_ends(tag_t curve, double start[3], double end[3])  
    {  
        double  
            junk[3];  
        UF_CALL(UF_MODL_ask_curve_props(curve, 0.0, start, junk, junk, junk, junk, junk));  
        UF_CALL(UF_MODL_ask_curve_props(curve, 1.0, end, junk, junk, junk, junk, junk));  
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
                            curves[kk] = curves[kk - 1];  
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
                            curves[kk] = curves[kk - 1];  
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
                        curves[jj] = curves[ii + 1];  
                        curves[ii + 1] = temp;  
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
                        curves[jj] = curves[ii + 1];  
                        curves[ii + 1] = temp;  
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
                start = ii + 1;  
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
                sprintf(msg, "%d", jj + 1);  
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
    static double ask_surface_area_of_sheet(tag_t sheet)  
    {  
        int  
            units;  
        tag_t  
            part = UF_PART_ask_display_part();  
        double  
            parameter[11] = { 0.99, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },  
            results[47],  
            statistics[13];  
        UF_CALL(UF_PART_ask_units(part, &units));  
        units = 5 - (2 * units); /*  UF_PART_METRIC  -> 3 = Grams and centimeters  
                                 UF_PART_ENGLISH -> 1 = Pounds and inches */  
        UF_CALL(UF_MODL_ask_mass_props_3d(&sheet, 1, 2, units, 1.0, 1, parameter,  
            results, statistics));  
        return results[0];  
    }  
    static logical ask_yes_or_no(char *title, char *message)  
    {  
        int  
            resp;  
        UF_UI_message_buttons_t  
            buttons = { TRUE, FALSE, TRUE, "Yes", NULL, "No", 1, 0, 2 };  
        UF_CALL(UF_UI_message_dialog(title, UF_UI_MESSAGE_QUESTION, &message, 1,  
            TRUE, &buttons, &resp));  
        if (resp == 1) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            inx = 0,  
            crvCount = 0,  
            mark_id;  
        logical  
            performUndo = FALSE;  
        tag_t  
            dPln = NULL_TAG,  
            theBody = NULL_TAG,  
            crvFeat = NULL_TAG,  
            bPlane = NULL_TAG,  
            *theCrvs = NULL,  
            set_1[1],  
            set_2[1];  
        UF_STRING_t  
            s_section;  
        double  
            tol[3],  
            surfaceArea = 0.;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, NULL_TAG, &mark_id));  
        UF_MODL_ask_distance_tolerance(&tol[0]);  
        UF_MODL_ask_angle_tolerance(&tol[1]);  
        dPln = select_a_datum_plane("Datum Plane:");  
        theBody = select_a_body("Body to Intersect:");  
        if (NULL_TAG == dPln || NULL_TAG == theBody) return;  
        set_1[0] = theBody;  
        set_2[0] = dPln;  
        UF_CALL(UF_CURVE_create_int_object(1, set_1, 1, set_2, &crvFeat));  
        if (NULL_TAG == crvFeat) return;  
        UF_CALL(UF_MODL_ask_feat_object(crvFeat, &crvCount, &theCrvs));  
        for (inx = 0; inx < crvCount; inx++)  
        {  
            UF_DISP_refresh();  
            sort_into_loops(theCrvs, crvCount, &s_section);  
            show_string_list_objects(&s_section);  
            UF_CALL(UF_MODL_create_bplane(&s_section, tol, &bPlane));  
            UF_MODL_free_string_list(&s_section);  
        }  
        if (crvCount > 0) UF_free(theCrvs);  
        ECHO("Sheet Body: %d\n\n", bPlane);  
        surfaceArea = ask_surface_area_of_sheet(bPlane);  
        ECHO("Area within intersection curves: %f\n", surfaceArea);  
        performUndo = ask_yes_or_no("Undo?", "Undo to remove curves and plane?");  
        if (performUndo == TRUE)  
        {  
            UF_CALL(UF_UNDO_undo_to_mark(mark_id, NULL_TAG));  
        }  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_TEXT_set_text_mode(UF_TEXT_ALL_UTF8));  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能包括：
>
> 1. 用户界面：定义了错误报告函数和提示框函数，用于错误提示和用户交互。
> 2. 选择体和基准面：实现了选择体的函数，以及选择基准面的函数，用于获取用户选择的实体。
> 3. 显示文本和曲线方向：实现了显示临时文本的函数，以及显示曲线方向和反转方向的函数，用于显示辅助信息。
> 4. 曲线排序和显示：实现了将曲线按连接关系排序成环的函数，以及显示排序后曲线的函数，用于对曲线进行排序和显示。
> 5. 创建曲面和面积计算：实现了创建曲面和计算曲面面积的函数，用于创建曲面并计算其面积。
> 6. 用户交互：实现了询问用户是否撤销的函数，用于与用户进行交互。
> 7. 主体函数：实现了主体函数，用于选择基准面和体，创建曲线，排序曲线，创建曲面，计算面积，并询问是否撤销。
> 8. 二次开发标准：实现了二次开发的入口函数，以及询问是否卸载函数，用于符合二次开发的标准。
>
> 总体来说，这段代码实现了从用户选择到曲面创建再到面积计算的一系列功能，并采用了NX提供的API进行开发，符合NX二次开发的标准流程。
>
