### 【2486】report spline thru pts 报告通过点的样条

#### 代码

```cpp
    /*HEAD REPORT_SPLINE_THRU_PTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_curve.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
    #define ECHOF(X)   (printf("%s = %f\n", #X, X))  
    #define ECHO3F(X)  (printf("%s = %f, %f, %f\n", #X, *X,*(X+1),*(X+2)))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中的新功能，用于打印系统日志信息。 */  
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
    static int mask_for_splines(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_spline_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_BCURVE_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_spline(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_splines, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            degree,  
            ii,  
            num_pts,  
            periodicity;  
        tag_t  
            spline;  
        double  
            *parms;  
        UF_CURVE_pt_slope_crvatr_t  
            *point_data;  
        while ((spline = select_a_spline("Select spline")) != NULL_TAG)  
        {  
            if (!UF_CALL(UF_CURVE_ask_spline_thru_pts(spline, &degree,  
                &periodicity, &num_pts, &point_data, &parms)))  
            {  
                ECHO(degree);  
                ECHO(periodicity);  
                ECHO(num_pts);  
                for (ii = 0; ii < num_pts + periodicity; ii++)  
                {  
                    ECHO(ii);  
                    ECHO3F(point_data[ii].point);  
                    ECHO(point_data[ii].slope_type);  
                    ECHOF(parms[ii]);  
                }  
                UF_free(point_data);  
                UF_free(parms);  
            }  
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

> 这段代码是一个用于查询样条曲线参数的NX Open C++二次开发示例程序。主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于打印错误信息。
> 2. 定义了一个选择样条曲线的函数select_a_spline，使用对话框让用户选择样条曲线。
> 3. 定义了一个主函数do_it，用于循环选择样条曲线，并查询每个样条曲线的参数，包括阶数、周期性、控制点数量、控制点坐标和斜率类型，并打印输出。
> 4. 定义了ufusr函数，作为程序的入口点，初始化和结束NX Open。
> 5. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示程序不需要卸载。
>
> 总的来说，这段代码通过NX Open C++ API，实现了对样条曲线参数的查询功能，并提供了友好的用户交互界面。
>
