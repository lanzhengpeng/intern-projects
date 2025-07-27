### 【1077】display curve tangents 显示曲线切线

#### 代码

```cpp
    /*HEAD DISPLAY_CURVE_TANGENTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_view.h>  
    #include <uf_trns.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 版本中的新功能。

在翻译方面，需要注意以下几点：
1. 确保翻译准确表达原文意思，避免歧义。
2. 使用简洁明了的语句，避免冗余的表达。
3. 保持术语的一致性，不要随意更改术语。
4. 在翻译时，确保理解原文背景，避免脱离上下文造成误解。

因此，对于UF_print_syslog的翻译，可以参考以下翻译结果：

UF_print_syslog 是 V18 版本中新增的函数。

希望以上翻译结果符合您的要求，如果需要进一步的修改或补充，请随时告知。 */  
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
    static void do_it(void)  
    {  
        tag_t  
            curve;  
        double  
            junk[3],  
            parm,  
            pos[3],  
            tangent[3];  
        while ((  
            curve = select_point_on_curve("Select curve point", pos)) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_curve_parm(curve, pos, &parm, pos));  
            UF_CALL(UF_MODL_ask_curve_props(curve, parm, pos, tangent, junk,  
                junk, junk, junk));  
            UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, pos, tangent, 0);  
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

> 这段NX二次开发代码的主要功能是选择曲线上的一点，计算该点的切线，并在该点处绘制一个锥头表示切线方向。具体步骤如下：
>
> 1. 定义了一些辅助函数，包括错误报告函数report_error()，用于将错误信息输出到系统日志和对话框。
> 2. 定义了两个坐标转换函数map_view2abs()和map_abs2view()，用于在绝对坐标系和视图坐标系之间转换坐标。
> 3. 定义了一个函数ask_pos_on_obj()，用于计算给定曲面与给定线的最小距离点。
> 4. 定义了一个函数mask_for_curves()，用于设置选择过滤条件，仅选择线、圆、锥曲线、样条线等曲线类型。
> 5. 定义了一个函数select_point_on_curve()，用于弹出选择对话框，让用户选择曲线上的一个点。
> 6. 定义了主函数do_it()，循环调用select_point_on_curve()选择曲线点，然后计算该点的切线方向，并在该点处绘制一个锥头表示切线方向。
> 7. 定义了ufusr()函数，作为NX的回调函数，在启动NX时调用do_it()函数执行主要功能。
> 8. 定义了ufusr_ask_unload()函数，用于卸载当前加载的库。
>
> 总体来说，这段代码通过NX的二次开发接口，实现了选择曲线点并显示其切线方向的功能。
>
