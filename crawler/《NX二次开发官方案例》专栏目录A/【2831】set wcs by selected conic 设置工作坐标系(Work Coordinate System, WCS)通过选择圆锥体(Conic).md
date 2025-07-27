### 【2831】set wcs by selected conic 设置工作坐标系(Work Coordinate System, WCS)通过选择圆锥体(Conic)

#### 代码

```cpp
    /*HEAD SET_WCS_BY_SELECTED_CONIC CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_csys.h>  
    #include <uf_eval.h>  
    #include <uf_mtx.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的函数。 */  
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
    /* qq3123197280 */  
    static int mask_for_conics(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_circle_type, 0, 0 },  
                        { UF_conic_type, UF_all_subtype, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_CIRCULAR_EDGE },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ELLIPTICAL_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    extern tag_t select_a_conic(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a conic", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_conics, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void set_wcs_by_x_axis_yaxis(double *x_axis, double *y_axis,  
        double *origin)  
    {  
        double  
            csys[9];  
        tag_t  
            mx,  
            wcs;  
        UF_CALL(UF_MTX3_initialize(x_axis, y_axis, csys));  
        UF_CALL(UF_CSYS_create_matrix(csys, &mx));  
        UF_CALL(UF_CSYS_create_temp_csys(origin, mx, &wcs));  
        UF_CALL(UF_CSYS_set_wcs(wcs));  
    }  
    static void set_wcs_by_conic_object(tag_t object)  
    {  
        logical  
            is_it;  
        UF_EVAL_p_t  
            eval;  
        UF_EVAL_arc_t  
            arc;  
        UF_EVAL_ellipse_t  
            ellipse;  
        UF_EVAL_parabola_t  
            prbola;  
        UF_EVAL_hyperbola_t  
            hyperbola;  
        UF_CALL(UF_EVAL_initialize(object, &eval));  
        UF_CALL(UF_EVAL_is_arc(eval, &is_it));  
        if (is_it)  
        {  
            UF_EVAL_ask_arc(eval, &arc);  
            set_wcs_by_x_axis_yaxis(arc.x_axis, arc.y_axis, arc.center);  
        }  
        UF_CALL(UF_EVAL_is_ellipse(eval, &is_it));  
        if (is_it)  
        {  
            UF_EVAL_ask_ellipse(eval, &ellipse);  
            set_wcs_by_x_axis_yaxis(ellipse.x_axis, ellipse.y_axis,  
                ellipse.center);  
        }  
        UF_CALL(UF_EVAL_is_parabola(eval, &is_it));  
        if (is_it)  
        {  
            UF_EVAL_ask_parabola(eval, &prbola);  
            set_wcs_by_x_axis_yaxis(prbola.x_axis, prbola.y_axis,  
                prbola.center);  
        }  
        UF_CALL(UF_EVAL_is_hyperbola(eval, &is_it));  
        if (is_it)  
        {  
            UF_EVAL_ask_hyperbola(eval, &hyperbola);  
            set_wcs_by_x_axis_yaxis(hyperbola.x_axis, hyperbola.y_axis,  
                hyperbola.center);  
        }  
        UF_CALL(UF_EVAL_free(eval));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            object;  
        while ((object = select_a_conic("Set WCS")) != NULL_TAG)  
            set_wcs_by_conic_object(object);  
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

> 这段代码是一个NX的二次开发代码，其功能是根据选择的圆锥面（圆、椭圆、抛物线或双曲线）来设置工作坐标系（WCS）。具体介绍如下：
>
> 1. 错误处理函数：report_error函数用于处理并报告函数调用错误，使用UF_print_syslog输出错误信息。
> 2. 选择圆锥面：mask_for_conics函数设置选择掩码，允许选择圆锥面。select_a_conic函数使用对话框让用户选择圆锥面，并高亮显示选择的圆锥面。
> 3. 设置坐标系：set_wcs_by_x_axis_yaxis函数根据输入的x轴、y轴和原点设置坐标系。set_wcs_by_conic_object函数根据圆锥面的参数设置坐标系。
> 4. 主函数：do_it函数循环调用select_a_conic函数，直到用户取消选择。对于每个选择的圆锥面，调用set_wcs_by_conic_object设置坐标系。
> 5. UF初始化：ufusr函数用于初始化和执行do_it函数，最后调用UF_terminate终止UF。
> 6. 卸载函数：ufusr_ask_unload函数用于立即卸载该UF。
>
> 通过以上函数，该代码实现了根据选择的圆锥面设置WCS的功能，提高了NX的交互性。
>
