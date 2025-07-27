### 【0836】create point set on curve using chordal tolerance 在曲线上使用弦公差创建点集

#### 代码

```cpp
    /*HEAD CREATE_POINT_SET_ON_CURVE_USING_CHORDAL_TOLERANCE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_curve.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog是V18版本新增的功能，用于打印系统日志。 */  
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
    extern tag_t select_a_curve_or_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve or edge", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves_and_edges, NULL,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strncpy(&menu[0][0], item, 16);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_pts;  
        tag_t  
            curve,  
            point;  
        double  
            ctol,  
            *pts;  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_MODL_ask_distance_tolerance(&ctol);  
        while ((curve = select_a_curve_or_edge("Points on Curve")) != NULL_TAG)  
        {  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, "Point Set", &mark_id));  
            while (prompt_for_a_number("Spacing - Chordal Tolerance",  
                "Tolerance", &ctol))  
            {  
                UF_CALL(UF_UNDO_undo_to_mark(mark_id, NULL));  
                UF_CALL(UF_MODL_ask_curve_points(curve, ctol, 0, 0, &n_pts, &pts));  
                for (ii = 0; ii < n_pts; ii++)  
                    UF_CALL(UF_CURVE_create_point(&pts[ii*3], &point));  
                if (n_pts > 0) UF_free(pts);  
            }  
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

> 这段代码是一个用于在曲线上创建点的NX Open C++ API应用程序。以下是代码的主要功能和步骤：
>
> 1. 包含必要的头文件：包括UF、UI、模型、曲线和显示模块的头文件。
> 2. 定义错误报告函数：用于在出错时打印错误信息。
> 3. 定义曲线和边界的过滤函数：用于在拾取操作时仅选择曲线和边界。
> 4. 选择曲线或边界的函数：用于打开选择对话框，让用户选择一个曲线或边界实体。
> 5. 提示输入距离公差的函数：用于提示用户输入距离公差。
> 6. 主函数do_it：不断提示用户选择曲线，然后提示输入距离公差，在曲线上创建点。
> 7. ufusr入口函数：初始化并调用do_it函数，然后终止。
> 8. 卸载函数：在应用程序卸载时立即返回。
>
> 综上所述，这段代码通过用户交互，在曲线上按给定的距离公差均匀创建点，适用于在曲线上进行点分布的二次开发需求。
>
