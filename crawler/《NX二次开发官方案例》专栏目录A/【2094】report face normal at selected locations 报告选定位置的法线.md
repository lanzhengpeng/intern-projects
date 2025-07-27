### 【2094】report face normal at selected locations 报告选定位置的法线

#### 代码

```cpp
    /*HEAD REPORT_FACE_NORMAL_AT_SELECTED_LOCATIONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_evalsf.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_view.h>  
    #include <uf_vec.h>  
    #include <uf_trns.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_BUFSIZE];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), _TRUNCATE, format, args);  
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
    static void map_abs2view(double c[3], tag_t view)  
    {  
        char  
            vname[30];  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(UF_OBJ_ask_name(view, vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(abs, vw, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void map_view2abs(double c[3], tag_t view)  
    {  
        char  
            vname[30];  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(UF_OBJ_ask_name(view, vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(vw, abs, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    /* qq3123197280 */  
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
    static tag_t select_point_on_face(char *prompt, double cp[3])  
    {  
        int  
            n,  
            resp;  
        tag_t  
            face,  
            line,  
            view;  
        double  
            tol;  
        UF_CURVE_line_t  
            lp;  
        UF_MODL_intersect_info_p_t  
            *ints;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &face, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(face, 0));  
            map_abs2view(cp, view);  
            lp.start_point[0] = cp[0];  
            lp.start_point[1] = cp[1];  
            lp.start_point[2] = cp[2] + 1000;  
            lp.end_point[0] = cp[0];  
            lp.end_point[1] = cp[1];  
            lp.end_point[2] = cp[2] - 1000;  
            map_view2abs(lp.start_point, view);  
            map_view2abs(lp.end_point, view);  
            UF_CALL(UF_CURVE_create_line(&lp, &line));  
            UF_MODL_ask_distance_tolerance(&tol);  
            UF_CALL(UF_MODL_intersect_objects(face, line, tol, &n, &ints));  
            if (n > 0)  
            {  
                UF_VEC3_copy(ints[0]->intersect.point.coords, cp);  
                UF_free(ints);  
            }  
            UF_CALL(UF_OBJ_delete_object(line));  
            return face;  
        }  
        else return NULL_TAG;  
    }  
    #define WRITE2F(X)  (write_double_array_to_listing_window(#X, X, 1, 2))  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    static void display_colored_conehead(double origin[3], double vec[3], int color)  
    {  
        UF_DISP_conehead_attrb_s  
            attrb;  
        UF_DISP_get_conehead_attrb(&attrb);  
        UF_CALL(UF_DISP_ask_closest_color_in_displayed_part(color, &attrb.color));  
        UF_DISP_set_conehead_attrb(&attrb);  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);  
        UF_DISP_reset_conehead_attrb();  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face;  
        double  
            loc[3],  
            junk[3],  
            modl_normal[3],  
            modl_parm[2],  
            modl_pt[3];  
        UF_EVALSF_p_t  
            eval;  
        UF_EVALSF_pos3_t  
            close_pos,  
            min_dist_pos;  
        UF_MODL_SRF_VALUE_t  
            close_pos_eval,  
            min_dist_eval;  
        while ((face = select_point_on_face("Display Normal", loc)) != NULL_TAG)  
        {  
            UF_CALL(UF_EVALSF_initialize(face, &eval));  
            UF_CALL(UF_EVALSF_find_closest_point(eval, loc, &close_pos));  
            WRITE2F(close_pos.uv);  
            WRITE3F(close_pos.pnt3);  
            UF_CALL(UF_EVALSF_evaluate(eval, UF_MODL_EVAL_ALL,  
                close_pos.uv, &close_pos_eval));  
            WRITE3F(close_pos_eval.srf_unormal);  
            WRITE3F(close_pos_eval.srf_normal);  
            display_colored_conehead(close_pos.pnt3, close_pos_eval.srf_unormal,  
                UF_DISP_BLUE_NAME);  
            display_colored_conehead(close_pos.pnt3, close_pos_eval.srf_normal,  
                UF_DISP_GREEN_NAME);  
            UF_CALL(UF_EVALSF_ask_minimum_face_dist(eval, loc, &min_dist_pos));  
            WRITE2F(min_dist_pos.uv);  
            WRITE3F(min_dist_pos.pnt3);  
            UF_CALL(UF_EVALSF_evaluate(eval, UF_MODL_EVAL_ALL,  
                min_dist_pos.uv, &min_dist_eval));  
            WRITE3F(min_dist_eval.srf_unormal);  
            WRITE3F(min_dist_eval.srf_normal);  
            display_colored_conehead(min_dist_pos.pnt3, min_dist_eval.srf_unormal,  
                UF_DISP_YELLOW_NAME);  
            display_colored_conehead(min_dist_pos.pnt3, min_dist_eval.srf_normal,  
                UF_DISP_RED_NAME);  
            UF_CALL(UF_EVALSF_free(&eval));  
            UF_CALL(UF_MODL_ask_face_parm(face, loc, modl_parm, modl_pt));  
            WRITE2F(modl_parm);  
            WRITE3F(modl_pt);  
            UF_CALL(UF_MODL_ask_face_props(face, modl_parm, junk, junk, junk, junk,  
                        junk, modl_normal, junk));  
            WRITE3F(modl_normal);  
            display_colored_conehead(modl_pt, modl_normal, UF_DISP_MAGENTA_NAME);  
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

> 这段NX二次开发代码主要实现了以下功能：
>
> 1. 选择模型面上的一个点，并获取该点在模型坐标系下的坐标。
> 2. 根据选择的点，使用NX提供的曲面评估函数UF_EVALSF，计算该点对应的曲面上的点坐标、参数坐标、法线方向等。
> 3. 在模型上以不同颜色显示该点的法线方向和两个最接近点的法线方向。
> 4. 获取该点在模型面上的参数坐标，以及模型面上的法线方向。
> 5. 在模型上以不同颜色显示这些法线方向。
> 6. 通过一个循环结构，可以重复选择点并显示相关信息。
> 7. 使用了UF_DISP的函数在模型上显示不同颜色的锥头，以直观显示法线方向。
> 8. 通过定义错误处理函数，方便地处理函数调用错误。
> 9. 在函数内部，使用了宏来简化代码，使代码更加简洁。
> 10. 在主函数ufusr中，初始化NX环境，调用主功能函数do_it，然后终止NX环境。
>
> 这段代码展示了NX二次开发中的常见功能，包括几何选择、曲面评估、法线显示等，对学习NX二次开发具有一定的参考价值。
>
