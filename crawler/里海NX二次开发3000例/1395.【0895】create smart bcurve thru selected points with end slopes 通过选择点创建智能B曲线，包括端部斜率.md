### 【0895】create smart bcurve thru selected points with end slopes 通过选择点创建智能B曲线，包括端部斜率

#### 代码

```cpp
    /*HEAD CREATE_SMART_BCURVE_THRU_SELECTED_POINTS_WITH_END_SLOPES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_so.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，翻译如下：

注意：UF_print_syslog是在V18版本中新增的，请直接回答翻译，不要添加其他无关内容。 */  
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
    static int mask_for_points(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_point_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_points(char *prompt, tag_t **points)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select points", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_points, NULL, &resp, &cnt, points));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*points)[ii], FALSE));  
        return cnt;  
    }  
    static logical specify_point(char *prompt, double loc[3])  
    {  
        int  
            ia2[2] = { 0,0 },  
            ip3 = 0,  
            irc;  
        irc = uc1616(prompt, ia2, ip3, loc);  
        if (irc == 5) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_pts;  
        tag_t  
            bcurve,  
            end_slope,  
            e_xyz[3],  
            *pts,  
            s_xyz[3],  
            start_slope;  
        double  
            e_pt[3],  
            e_spt[3],  
            e_vec[3],  
            s_pt[3],  
            s_spt[3],  
            s_vec[3];  
        if (((n_pts = select_points("smart bcurve thru points", &pts)) > 0) &&  
            specify_point("Start slope toward", s_spt) &&  
            specify_point("End slope toward", e_spt))  
        {  
            UF_CALL(UF_CURVE_ask_point_data(pts[0], s_pt));  
            UF_CALL(UF_CURVE_ask_point_data(pts[n_pts-1], e_pt));  
            UF_VEC3_sub(s_pt, s_spt, s_vec);  
            UF_VEC3_sub(e_spt, e_pt, e_vec);  
            for (ii = 0; ii < 3; ii++)  
            {  
                UF_CALL(UF_SO_create_scalar_double(pts[0],  
                    UF_SO_update_after_modeling, s_vec[ii], &s_xyz[ii]));  
                UF_CALL(UF_SO_create_scalar_double(pts[0],  
                    UF_SO_update_after_modeling, e_vec[ii], &e_xyz[ii]));  
            }  
            UF_CALL(UF_SO_create_point_3_scalars(pts[0],  
                UF_SO_update_after_modeling, s_xyz, &start_slope));  
            UF_CALL(UF_SO_create_point_3_scalars(pts[0],  
                UF_SO_update_after_modeling, e_xyz, &end_slope));  
            UF_CALL(UF_SO_create_bcurve_thru_points(UF_SO_update_after_modeling,  
                n_pts, pts, NULL, 3, 0, start_slope, end_slope,  
                UF_SO_bcurve_slope_direction, UF_SO_bcurve_slope_direction,  
                &bcurve));  
            UF_CALL(UF_SO_set_visibility_option( bcurve, UF_SO_visible));  
            UF_free(pts);  
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

> 这段代码是NX Open C++ API编写的NX二次开发代码，其主要功能是创建一个智能曲线通过选定的点，并指定起始和结束斜率。
>
> 主要流程如下：
>
> 1. 定义了错误报告函数report_error，用于在函数调用失败时报告错误。
> 2. 定义了选择点的函数select_points，用于选择工作部件中的点。
> 3. 定义了指定点的函数specify_point，用于让用户指定一个点的坐标。
> 4. 定义了主函数do_it，用于执行以下流程：选择工作部件中的点指定起始斜率的方向指定结束斜率的方向生成起始和结束斜率向量创建智能曲线设置曲线的可见性
> 5. 选择工作部件中的点
> 6. 指定起始斜率的方向
> 7. 指定结束斜率的方向
> 8. 生成起始和结束斜率向量
> 9. 创建智能曲线
> 10. 设置曲线的可见性
> 11. 定义了ufusr函数，作为程序的入口点，用于初始化和执行do_it函数，以及终止NX Open API。
> 12. 定义了ufusr_ask_unload函数，用于卸载用户自定义函数。
>
> 通过以上步骤，这段代码实现了通过选定点创建智能曲线，并指定起始和结束斜率的功能。
>
