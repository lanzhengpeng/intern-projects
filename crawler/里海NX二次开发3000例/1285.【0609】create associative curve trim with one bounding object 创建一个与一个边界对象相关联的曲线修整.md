### 【0609】create associative curve trim with one bounding object 创建一个与一个边界对象相关联的曲线修整

#### 代码

```cpp
    /*HEAD CREATE_ASSOCIATIVE_CURVE_TRIM_WITH_ONE_BOUNDING_OBJECT CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_vec.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
    #include <uf_view.h>  
    #include <uf_trns.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本新增的功能，用于打印系统日志信息。 */  
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
    static int ask_curve_close_end(tag_t curve, double pos[3])  
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
        if (dist[0] < dist[1]) return 1;  
        else return 2;  
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
    static tag_t select_a_curve_end(char *prompt, int *which_end)  
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
            *which_end = ask_curve_close_end(object, pos);  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
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
    static void do_it(void)  
    {  
        int  
            which_end;  
        tag_t  
            trim_feat;  
        UF_CURVE_trim_t  
            trim_info;  
        UF_CURVE_trim_mult_t  
            out_info;  
        UF_MODL_init_string_list(&trim_info.string_to_trim);  
        UF_MODL_create_string_list(1, 1, &trim_info.string_to_trim);  
        trim_info.string_to_trim.string[0] = 1;  
        trim_info.string_to_trim.dir[0] = 1;  
        trim_info.string_to_trim.id[0] = NULL_TAG;  
        trim_info.spline_extend_opt = UF_CURVE_EXTEND_NATURAL;  
        trim_info.trim_type = UF_CURVE_TRIM_TO_ONE_BOUND;  
        UF_MODL_ask_distance_tolerance(&trim_info.tolerances[0]);  
        UF_MODL_ask_angle_tolerance(&trim_info.tolerances[1]);  
        UF_MODL_init_string_list(&trim_info.trim_to.one_bound.bound.string);  
        trim_info.trim_to.one_bound.bound.object = NULL_TAG;  
        trim_info.trim_to.one_bound.bound.use_suggested = FALSE;  
        trim_info.trim_to.one_bound.trim_bound = FALSE;  
        trim_info.trim_to.one_bound.view = NULL;  
        while (((trim_info.string_to_trim.id[0] =  
                select_a_curve_end("Select curve to trim", &which_end))  
                    != NULL_TAG) &&  
                ((trim_info.trim_to.one_bound.bound.object =  
                    select_a_face("Select bounding object")) != NULL_TAG))  
        {  
            if (which_end == 1)  
            {  
                    trim_info.trim_to.one_bound.string_trim_extend_end =  
                        UF_CURVE_TRIM_EXTEND_START;  
            }  
            else  
            {  
                    trim_info.trim_to.one_bound.string_trim_extend_end =  
                        UF_CURVE_TRIM_EXTEND_END;  
            }  
            set_undo_mark("Trim Curve");  
        /*  This function causes a circular update error starting in NX 5  
            see PR 1689882 */  
            if (UF_CALL(UF_CURVE_create_trim(&trim_info, &out_info, &trim_feat))  
                == UF_CURVE_TRIM_MULT_PTS)  
            {  
                if (out_info.num_bound1_pts > 0) UF_free(out_info.bound1_pts);  
                if (out_info.num_bound2_pts > 0) UF_free(out_info.bound2_pts);  
            }  
            UF_CALL(UF_MODL_update());  
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

> 这段代码是NX二次开发的一个示例，主要实现了创建曲线修剪特征的功能。以下是代码的主要内容和功能：
>
> 1. 包含了NX二次开发所需的头文件，如uf.h、uf_ui.h等。
> 2. 定义了错误处理函数report_error，用于打印UF函数调用的错误信息。
> 3. 定义了选择面的函数select_a_face，以及用于将绝对坐标系坐标转换为视图坐标系的函数map_abs2view和map_view2abs。
> 4. 定义了选择曲线端点的函数select_a_curve_end，以及确定曲线端点距离输入位置最近的函数ask_curve_close_end。
> 5. 定义了设置撤销标记的函数set_undo_mark。
> 6. 定义了主要函数do_it，用于创建曲线修剪特征。函数中首先初始化修剪信息结构体，然后进入循环，在循环中让用户选择要修剪的曲线和边界对象，设置修剪方向，创建撤销标记，然后调用UF_CURVE_create_trim创建修剪特征。
> 7. 定义了ufusr函数，用于调用do_it函数实现主要功能。
> 8. 定义了ufusr_ask_unload函数，用于设置二次开发程序的卸载模式。
>
> 总体来说，这段代码实现了选择曲线和面，创建曲线修剪特征的功能，具有一定的参考价值。但需要注意，代码中某些函数可能在NX 5之后的版本中存在兼容性问题。
>
