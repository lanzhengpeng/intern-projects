### 【1119】create daxis using a curve 使用曲线创建法线方向，即创建一个曲线的法线方向轴

#### 代码

```cpp
    /*HEAD CREATE_DAXIS_USING_A_CURVE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_trns.h>  
    #include <uf_modl.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是在V18版本中新增的。因此，UF_print_syslog在V18版本中是新的。 */  
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
    static void map_view2abs(double c[3])  
    {  
        char  
            vname[30];  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(vw, abs, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void map_abs2view(double c[3])  
    {  
        char  
            vname[30];  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(uc1653(vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(abs, vw, mx, &irc);  
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
    static tag_t select_point_on_curve(char *prompt, double cp[3])  
    {  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            ask_pos_on_obj(object, cp);  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical ask_yes_or_no(char *prompt, logical *deflt)  
    {  
        int  
            resp;  
        char  
            options[2][38] = { "Yes", "No" };  
        resp = 2 - *deflt;  
        resp = uc1603(prompt, resp, options, 2);  
        if (resp > 4)  
        {  
            *deflt = 6 - resp;  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        logical  
            ok = FALSE;  
        int  
            ii,  
            is_zero,  
            subtype,  
            type;  
        tag_t  
            curve,  
            daxis,  
            temp;  
        double  
            b_norm[3],  
            dir[3],  
            i_len,  
            p_norm[3],  
            parm,  
            point[3],  
            pos[3],  
            radius,  
            t_len,  
            tangent[3],  
            tol,  
            torsion;  
        char  
            perc[UF_MAX_EXP_LENGTH+1];  
        UF_MODL_ask_distance_tolerance(&tol);  
        while ((curve = select_point_on_curve("Select curve", pos)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(curve, &type, &subtype));  
            if (type == UF_solid_type)  
            {  
                UF_CALL(UF_MODL_create_curve_from_edge(curve, &temp));  
                UF_CALL(UF_MODL_ask_curve_parm(temp, pos, &parm, point));  
                UF_CALL(UF_OBJ_delete_object(temp));  
            }  
            else  
                UF_CALL(UF_MODL_ask_curve_parm(curve, pos, &parm, point));  
            UF_CALL(UF_MODL_ask_curve_props(curve, parm, point, tangent, p_norm,  
                b_norm, &torsion, &radius));  
            ii = 0;  
            do  
            {  
                ii++;  
                if (ii == 7) ii = 1;  
                switch (ii)  
                {  
                    case 1:  
                        UF_VEC3_copy(tangent, dir);  
                        break;  
                    case 2:  
                        UF_VEC3_copy(p_norm, dir);  
                        break;  
                    case 3:  
                        UF_VEC3_copy(b_norm, dir);  
                        break;  
                    case 4:  
                        UF_VEC3_negate(tangent, dir);  
                        break;  
                    case 5:  
                        UF_VEC3_negate(p_norm, dir);  
                        break;  
                    case 6:  
                        UF_VEC3_negate(b_norm, dir);  
                        break;  
                }  
                UF_DISP_conehead(UF_DISP_WORK_VIEW_ONLY, pos, dir, 0);  
                UF_VEC3_is_zero(dir, tol, &is_zero);  
            } while ((is_zero == 1) || ask_yes_or_no("This one?", &ok) && !ok);  
            UF_CALL(UF_CURVE_ask_arc_length(curve, 0.0, 1.0, UF_MODL_UNITS_PART,  
                &t_len));  
            UF_CALL(UF_CURVE_ask_arc_length(curve, 0.0, parm, UF_MODL_UNITS_PART,  
                &i_len));  
            parm = 100 * (i_len/t_len);  
            sprintf(perc, "%f", parm);  
            UF_CALL(UF_MODL_create_frenet_daxis(curve, NULL_TAG, perc, ii, &daxis));  
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

> 这段代码是一个NX Open C++ API编写的二次开发程序，主要功能是创建曲线的法线方向基准轴。
>
> 1. 代码结构：包括头文件包含、错误报告函数、视图坐标系转换函数、在曲线上询问位置函数、曲线选择遮罩函数、选择曲线上的点函数、询问用户函数、主函数、用户函数接口和卸载函数。
> 2. 主函数：do_it()是主函数，循环选择曲线上的点，计算法线、副法线、主曲率方向、它们的负方向，并询问用户选择一个方向，然后在该位置创建一个法线方向的基准轴。
> 3. 错误处理：使用错误报告函数report_error()来处理NX API调用错误。
> 4. 坐标系转换：使用视图坐标系转换函数map_view2abs()和map_abs2view()来在视图坐标系和绝对坐标系之间转换点。
> 5. 用户交互：使用询问用户函数ask_yes_or_no()来询问用户问题。
> 6. 接口：ufusr()是用户函数接口，用于初始化和终止NX Open，并调用主函数。ufusr_ask_unload()用于立即卸载二次开发程序。
>
> 这段代码展示了如何利用NX Open C++ API进行曲线操作、坐标系转换、用户交互等，具有一定的参考价值。
>
