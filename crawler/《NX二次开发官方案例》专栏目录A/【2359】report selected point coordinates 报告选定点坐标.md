### 【2359】report selected point coordinates 报告选定点坐标

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_POINT_COORDINATES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_curve.h>  
    #include <uf_csys.h>  
    #include <uf_trns.h>  
    #include <uf_vec.h>  
    #define ECHO3F(X)  (printf("\t%s = %f, %f, %f\n", #X, *X,*(X+1),*(X+2)))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，翻译为：

注意：UF_print_syslog 是 V18 新增的功能，请只回答与译文相关的内容，不要添加无关内容。 */  
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
    static void map_abs2wcs(double *cc)  
    {  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            wcs[12],  
            mx[12];  
        int  
            irc;  
        tag_t  
            wcs_ent,  
            wcs_mx;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs_ent));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs_ent, &wcs_mx, wcs));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, &wcs[3]));  
        FTN(uf5940)(abs, wcs, mx, &irc);  
        FTN(uf5941)(cc, mx);  
    }  
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
        tag_t  
            *objs;  
        UF_CALL(UF_UI_select_with_class_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_points, NULL, &resp, &cnt, points));  
        objs = *points;  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight(objs[ii], FALSE));  
        return cnt;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *points;  
        double  
            pt_abs[3],  
            pt_wcs[3];  
        while ((n = select_points("Select points", &points)) > 0)  
        {  
            for (ii = 0; ii < n; ii++)  
            {  
                UF_CALL(UF_CURVE_ask_point_data(points[ii], pt_abs));  
                ECHO3F(pt_abs);  
                UF_VEC3_copy(pt_abs, pt_wcs);  
                map_abs2wcs(pt_wcs);  
                ECHO3F(pt_wcs);  
            }  
            UF_free(points);  
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

> 这段代码是一个用于NX的二次开发程序，其主要功能是选择并报告点的绝对坐标和世界坐标。以下是该代码的主要功能和步骤：
>
> 1. 头文件引入：引入了必要的NX API头文件，如uf.h、uf_ui.h等。
> 2. 错误报告函数：定义了report_error函数，用于在函数调用失败时输出错误信息。
> 3. 坐标系转换函数：定义了map_abs2wcs函数，用于将绝对坐标系下的点转换到世界坐标系下。
> 4. 选择点函数：定义了mask_for_points函数，用于设置选择点时的过滤条件。
> 5. 选择点并报告坐标：定义了select_points函数，用于选择点并返回点集。在主函数do_it中，循环调用select_points函数，选择点集后，遍历每个点，获取其绝对坐标，然后转换到世界坐标系下，并打印出两个坐标系的坐标。
> 6. 主函数：定义了ufusr函数，用于初始化NX环境，调用do_it函数执行主体功能，并在结束时关闭NX环境。
> 7. 卸载函数：定义了ufusr_ask_unload函数，用于设置程序卸载方式为立即卸载。
>
> 通过以上步骤，该程序实现了在NX中选择点，并报告每个点在绝对坐标系和世界坐标系下的坐标。
>
