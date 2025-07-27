### 【0844】create points on curve 在曲线上创建点

#### 代码

```cpp
    /*HEAD CREATE_POINTS_ON_CURVE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_object_types.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_undo.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：

注意：UF_print_syslog 是 V18 新增的。 */  
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
    extern tag_t select_a_curve(char *prompt)  
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
    static logical ask_tols(double tols[3])  
    {  
        int  
            junk,  
            resp;  
        char  
            prompt[][16] = { "Chord tol", "Angle tol", "Max length" };  
        resp = uc1609("Enter point set parameters", prompt, 3, tols, &junk);  
        if (resp == 3 || resp == 4) return TRUE;  
        else return FALSE;  
    }  
    static void process(double tols[3], tag_t curve)  
    {  
        char  
            name[30];  
        double  
            angle,  
            dist,  
            gp[3],  
            junk[3],  
            last[3],  
            parm,  
            *pts,  
            tanv[3];  
        int  
            color = 1,  
            i,j,  
            np;  
        tag_t  
            point;  
        if (!UF_CALL(UF_MODL_ask_curve_points(curve, tols[0], tols[1], tols[2],  
            &np, &pts)))  
        {  
            printf("Curve tag = %d\tNumber of points = %d\n", curve, np);  
            printf("\tChord tol\t=\t%f\n", tols[0]);  
            printf("\tAngle tol\t=\t%f\n", tols[1]);  
            printf("\tMax length\t=\t%f\n", tols[2]);  
            for (i=0; i<np; i++)  
            {  
                j = i * 3;  
                UF_CALL(UF_CURVE_create_point(&pts[j], &point));  
                UF_CALL(UF_OBJ_set_color(point, color++));  
                if (color == 11) color = 1;  
                UF_CALL(UF_MODL_ask_curve_parm(curve, &pts[j], &parm, gp));  
                UF_CALL(UF_MODL_ask_curve_props(curve, parm, gp, tanv, junk, junk,  
                    junk, junk));  
                if (i > 0)  
                {  
                    gp[0] = (pts[j] + pts[j-3])/2;  
                    gp[1] = (pts[j+1] + pts[j-2])/2;  
                    gp[2] = (pts[j+2] + pts[j-1])/2;  
                    UF_CALL(UF_CURVE_create_point(gp, &point));  
                    UF_CALL(UF_MODL_ask_minimum_dist(point, curve, 1, gp, 0, gp,  
                        &dist, gp, gp));  
                    UF_CALL(UF_MODL_ask_vector_angle(tanv, last, &angle, junk));  
                    if ((dist  > tols[0] && tols[0] > 0) ||  
                        (angle > tols[1] && tols[1] > 0))  
                    {  
                        printf("\t%d. Chord\t=\t%f\n", i, dist);  
                        printf("\t    Angle\t=\t%f\n", angle);  
                        UF_CALL(UF_OBJ_set_color(point, UF_OBJ_GRAY));  
                        sprintf(name, "%d", i);  
                        UF_CALL(UF_OBJ_set_name(point, name));  
                    }  
                    else  
                    {  
                        UF_CALL(UF_OBJ_delete_object(point));  
                    }  
                }  
                last[0] = tanv[0];  
                last[1] = tanv[1];  
                last[2] = tanv[2];  
            }  
            if (np > 0) UF_free(pts);  
        }  
    }  
    static void do_it(void)  
    {  
        double  
            tols[3] = { .01, .1, 1 };  
        tag_t  
            curve;  
        UF_UNDO_mark_id_t  
            mark;  
        while ((curve = select_a_curve("Select a curve")) != NULL_TAG)  
        {  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, NULL, &mark));  
            while (ask_tols(tols))  
            {  
                UF_CALL(UF_UNDO_undo_to_mark(mark, NULL));  
                process(tols, curve);  
            }  
        }  
    }  
    /* 里海 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是用于NX的二次开发，其功能是在曲线上创建点集。
>
> 主要步骤如下：
>
> 1. 定义错误报告函数report_error，用于打印错误信息。
> 2. 定义曲线选择函数select_a_curve，用于在对话框中选择一条曲线，并高亮显示。
> 3. 定义参数输入函数ask_tols，用于在对话框中输入点集参数。
> 4. 定义处理函数process，用于在曲线上创建点集。参数包括弦公差、角度公差和最大长度。根据这些参数，在曲线上均匀生成点，并创建相应的点对象，并计算相邻点的弦长和夹角，判断是否满足公差条件，不满足则删除点。
> 5. 主函数do_it中，循环选择曲线，设置撤销标记，循环输入参数，调用process函数处理，然后撤销到标记处。
> 6. ufusr函数是NX调用的主函数，用于初始化和终止NX，并调用do_it函数。
> 7. ufusr_ask_unload函数用于设置立即卸载二次开发代码。
>
> 综上，这段代码通过交互方式，让用户选择曲线并输入参数，在曲线上均匀生成点集，以满足特定公差条件，方便后续的曲线处理。
>
