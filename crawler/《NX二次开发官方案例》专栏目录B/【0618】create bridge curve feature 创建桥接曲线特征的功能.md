### 【0618】create bridge curve feature 创建桥接曲线特征的功能

#### 代码

```cpp
    /*HEAD CREATE_BRIDGE_CURVE_FEATURE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
    #include <uf_trns.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本新增的函数。 */  
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
        UF_CALL(UF_DISP_set_display(FALSE));  
        UF_CALL(UF_CURVE_create_line(&lp, &line));  
        UF_CALL(UF_MODL_ask_minimum_dist(obj, line, 0, cp, 0, cp, &dist, loc, cp));  
        UF_CALL(UF_OBJ_delete_object(line));  
        UF_CALL(UF_DISP_set_display(TRUE));  
    }  
    static double ask_curve_close_end_parm(tag_t curve, double pos[3])  
    {  
        double  
            dist[2],  
            end[3],  
            junk[3],  
            start[3];  
        UF_CALL(UF_MODL_ask_curve_props(curve, 0.0, start, junk, junk, junk,  
            junk, junk));  
        UF_CALL(UF_MODL_ask_curve_props(curve, 1.0, end, junk, junk, junk,  
            junk, junk));  
        UF_VEC3_distance(pos, start, &dist[0]);  
        UF_VEC3_distance(pos, end, &dist[1]);  
        if (dist[0] < dist[1]) return 0.0;  
        else return 1.0;  
    }  
    /* 里海 */  
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_curve_end_parm(char *prompt, double *parm)  
    {  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        double  
            pos[3];  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve end", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves, NULL, &resp,  
            &object, pos, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            ask_pos_on_obj(object, pos);  
            *parm = ask_curve_close_end_parm(object, pos);  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITEnL(X,n) (write_logical_array_to_listing_window(#X, X, n))  
    void write_logical_array_to_listing_window(char *title, logical *logicals,  
        int n_logicals)  
    {  
        int  
            ii;  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        for (ii = 0; ii < n_logicals; ii++)  
        {  
            if (logicals[ii])  
                sprintf(msg, "%s[%d] = TRUE\n", title, ii);  
            else  
                sprintf(msg, "%s[%d] = FALSE\n", title, ii);  
            UF_CALL(UF_UI_write_listing_window(msg));  
        }  
    }  
    #define WRITEnD(n,X) (write_integer_array_to_listing_window(#X, X, 1, n))  
    static void write_integer_array_to_listing_window(char *name, int *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
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
                sprintf(msg, "%d", array[kk++]);  
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
    #define WRITEnS(X,n) (write_string_array_to_listing_window(#X, X, n))  
    void write_string_array_to_listing_window(char *title, char **strings,  
        int n_strings)  
    {  
        int  
            ii;  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        for (ii = 0; ii < n_strings; ii++)  
        {  
            sprintf(msg, "%s[%d] = %s\n", title, ii, strings[ii]);  
            UF_CALL(UF_UI_write_listing_window(msg));  
        }  
    }  
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
    static void report_bridge_curve_feature_data( UF_CURVE_bridge_data_p_t  
            bridge_data)  
    {  
        WRITE_D(bridge_data->method);  
        WRITE_SL(bridge_data->input_curve1);  
        WRITE_SL(bridge_data->input_curve2);  
        WRITEnS(bridge_data->matchpt_parms, 2);  
        WRITEnD(2, (int *)bridge_data->match_point);  
        WRITEnL(bridge_data->reverse_tangents, 2);  
        WRITE_S(bridge_data->shape_control1);  
        WRITE_S(bridge_data->shape_control2);  
        WRITE_D(bridge_data->stiffness_method);  
        if (bridge_data->inherit_curve != NULL)  
        {  
            WRITE_SL(bridge_data->inherit_curve);  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feature;  
        double  
            match_parms[2];  
        char  
            mp1_exp[10],  
            mp2_exp[10];  
        UF_STRING_t  
            input_curve1,  
            input_curve2;  
        UF_CURVE_bridge_data_t  
            bridge_data;  
        bridge_data.method = UF_CURVE_match_tangent_ends;  
        bridge_data.input_curve1 = &input_curve1;  
        UF_MODL_init_string_list(bridge_data.input_curve1);  
        UF_MODL_create_string_list(1, 1, bridge_data.input_curve1);  
        bridge_data.input_curve2 = &input_curve2;  
        UF_MODL_init_string_list(bridge_data.input_curve2);  
        UF_MODL_create_string_list(1, 1, bridge_data.input_curve2);  
        bridge_data.matchpt_parms[0] = mp1_exp;  
        bridge_data.matchpt_parms[1] = mp2_exp;  
        bridge_data.match_point[0] = NULL_TAG;  
        bridge_data.match_point[1] = NULL_TAG;  
        bridge_data.reverse_tangents[0] = TRUE;  
        bridge_data.reverse_tangents[1] = TRUE;  
        bridge_data.shape_control1 = "1.0";  
        bridge_data.shape_control2 = "1.0";  
        bridge_data.stiffness_method = 0;  
        bridge_data.inherit_curve = NULL;  
        if (((bridge_data.input_curve1->id[0] = select_a_curve_end_parm(  
            "Bridge curve 1", &match_parms[0])) != NULL_TAG) &&  
            ((bridge_data.input_curve2->id[0] = select_a_curve_end_parm(  
            "Bridge curve 2", &match_parms[1])) != NULL_TAG))  
        {  
            bridge_data.input_curve1->string[0] = 1;  
            bridge_data.input_curve1->dir[0] = 1;  
            bridge_data.input_curve2->string[0] = 1;  
            bridge_data.input_curve2->dir[0] = 1;  
            sprintf(bridge_data.matchpt_parms[0], "%f", match_parms[0] * 100.0);  
            sprintf(bridge_data.matchpt_parms[1], "%f", match_parms[1] * 100.0);  
            report_bridge_curve_feature_data(&bridge_data);  
            UF_CALL(UF_CURVE_create_bridge_feature(&bridge_data, &feature));  
        }  
        UF_MODL_free_string_list(bridge_data.input_curve1);  
        UF_MODL_free_string_list(bridge_data.input_curve2);  
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

> 这段代码是用于NX的二次开发，其主要功能是创建一个桥接曲线特征。
>
> 主要步骤包括：
>
> 1. 定义了错误处理函数report_error，用于打印错误信息。
> 2. 定义了坐标变换函数map_abs2view和map_view2abs，用于将绝对坐标系下的坐标转换到视图坐标系下，以及相反的转换。
> 3. 定义了ask_pos_on_obj函数，用于在指定对象上拾取位置。
> 4. 定义了ask_curve_close_end_parm函数，用于获取曲线端点参数。
> 5. 定义了mask_for_curves函数，用于设置选择过滤条件，仅允许选择线、圆、锥和样条曲线。
> 6. 定义了select_a_curve_end_parm函数，用于选择一个曲线端点，并返回曲线和端点参数。
> 7. 定义了各种写入信息到日志窗口的函数。
> 8. 定义了显示曲线方向的函数show_curve_direction和show_curve_flip_direction。
> 9. 定义了报告桥接曲线数据函数report_bridge_curve_feature_data。
> 10. 在do_it函数中，初始化桥接曲线数据，选择两个曲线端点，设置桥接参数，然后创建桥接曲线特征。
> 11. 在ufusr函数中调用do_it函数进行桥接曲线的创建。
> 12. 最后定义了卸载函数ufusr_ask_unload。
>
> 总体来说，这段代码通过UI交互选择两个曲线端点，设置桥接参数，并最终创建出桥接曲线特征。
>
