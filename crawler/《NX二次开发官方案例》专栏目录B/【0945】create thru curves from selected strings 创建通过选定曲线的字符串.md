### 【0945】create thru curves from selected strings 创建通过选定曲线的字符串

#### 代码

```cpp
    /*HEAD CREATE_THRU_CURVES_FROM_SELECTED_STRINGS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_curve.h>  
    #include <uf_vec.h>  
    #include <uf_view.h>  
    #include <uf_trns.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：UF_print_syslog 是 V18 新增的。

只回答译文，不要废话。 */  
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
    /* qq3123197280 */  
    static int mask_for_curves_and_edges(UF_UI_selection_p_t select, void *type)  
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
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves_and_edges, NULL, &resp,  
            &object, pos, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            ask_pos_on_obj(object, pos);  
            UF_CALL(UF_MODL_get_curve_edge_direction(pos, object, which_end));  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical specify_strings(char *which, UF_STRING_t *string)  
    {  
        int  
            dirs[50],  
            ii = 0,  
            jj = 0,  
            kk = -1,  
            n,  
            n_segs[10];  
        char  
            prompt[81];  
        tag_t  
            curves[50];  
        UF_MODL_init_string_list(string);  
        do  
        {  
            if ((kk > -1) && (curves[kk] == NULL_TAG))  
            {  
                n_segs[ii] = jj-1;  
                ii++;  
                jj = 0;  
                kk--;  
            }  
            if (jj > 0)  
            {  
                sprintf(prompt, "OK to finish selecting string #%d", ii+1);  
                UF_CALL(UF_UI_set_status(prompt));  
            }  
            else if (ii > 0)  
            {  
                sprintf(prompt, "OK to finish selecting %s strings", which);  
                UF_CALL(UF_UI_set_status(prompt));  
            }  
            else  
                UF_CALL(UF_UI_set_status(""));  
            sprintf(prompt, "Select %s String #%d - curve #%d", which, ii+1, ++jj);  
            kk++;  
            curves[kk] = select_a_curve_end(prompt, &dirs[kk]);  
            if (curves[kk] != NULL_TAG)  
                UF_CALL(UF_DISP_set_highlight(curves[kk], TRUE));  
        } while ((curves[kk] != NULL_TAG) || jj > 1);  
        UF_CALL(UF_UI_set_status(""));  
        if (kk > 0)  
        {  
            n_segs[ii] = jj;  
            n = ii;  
            UF_MODL_create_string_list(n, kk, string);  
            kk = 0;  
            for (ii = 0; ii < n; ii++)  
            {  
                string->string[ii] = n_segs[ii];  
                string->dir[ii] = dirs[kk];  
                for (jj = 0; jj < n_segs[ii]; jj++, kk++)  
                {  
                    string->id[kk] = curves[kk];  
                    UF_CALL(UF_DISP_set_highlight(curves[kk], FALSE));  
                }  
            }  
            return TRUE;  
        }  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
         int  
            alignment = 1,  
            body_type = 1,  
            c_flag[2] = { 0,0 },  
            patch = 2,  
            vdegree = 3,  
            vstatus = 0;  
        double  
            tol[3] = { 0.001, 0.5, 0.001 },  
            value[6] = { 0,0,0, 0,0,0 };  
        tag_t  
            body_id,  
            c_face[2] = { NULL_TAG, NULL_TAG };  
        UF_STRING_t  
            s_section,  
            s_spine;  
        UF_UNDO_mark_id_t  
            mark;  
        UF_MODL_init_string_list(&s_spine);  
        if (specify_strings("section", &s_section))  
        {  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, "Create thru curves", &mark));  
            UF_CALL(UF_MODL_create_thru_curves( &s_section, &s_spine, &patch,  
                &alignment, value, &vdegree, &vstatus, &body_type, UF_NULLSIGN,  
                tol, c_face, c_flag, &body_id ));  
            UF_MODL_free_string_list(&s_section);  
        }  
        UF_MODL_free_string_list(&s_spine);  
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

> 这段代码是用于NX的二次开发，主要实现从选择曲线创建通过曲线的功能。具体来说，代码主要完成以下工作：
>
> 1. 包含了必要的NX API头文件，定义了错误报告函数report_error()。
> 2. 定义了两个转换函数map_abs2view()和map_view2abs()，用于视图坐标系和绝对坐标系之间的转换。
> 3. 定义了ask_pos_on_obj()函数，用于在指定对象上询问位置。
> 4. 定义了mask_for_curves_and_edges()函数，用于设置选择掩码，只允许选择曲线和实体边缘。
> 5. 定义了select_a_curve_end()函数，用于选择曲线端点并获取方向。
> 6. 定义了specify_strings()函数，用于指定截面和脊柱曲线字符串。
> 7. do_it()函数实现了主要功能：首先指定截面曲线字符串，然后调用UF_MODL_create_thru_curves() API创建通过曲线。
> 8. ufusr()函数是二次开发的入口函数，调用do_it()函数实现功能。
> 9. ufusr_ask_unload()函数用于卸载二次开发。
>
> 总的来说，这段代码通过二次开发实现了从选择的曲线创建通过曲线的功能，用户可以通过选择截面和脊柱曲线来生成所需的曲线。
>
