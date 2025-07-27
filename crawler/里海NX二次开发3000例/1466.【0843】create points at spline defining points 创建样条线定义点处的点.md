### 【0843】create points at spline defining points 创建样条线定义点处的点

#### 代码

```cpp
    /*HEAD CREATE_POINTS_AT_SPLINE_DEFINING_POINTS CCC UFUN*/   
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：UF_print_syslog是在V18版本中新增的函数。 */  
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
            spline,  
            point;  
        double  
            *parms;  
        UF_CURVE_pt_slope_crvatr_t  
            *point_data;  
        while ((spline = select_a_spline("Select spline")) != NULL_TAG)  
        {  
            if (!UF_CALL(UF_CURVE_ask_spline_thru_pts(spline, &degree,  
                &periodicity, &num_pts, &point_data, &parms)))  
            {  
                for (ii = 0; ii < num_pts; ii++)  
                {  
                    UF_CALL(UF_CURVE_create_point(point_data[ii].point, &point));  
                }  
                UF_free(point_data);  
                UF_free(parms);  
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

> 这段代码是NX的二次开发代码，主要功能是在NX中创建一个对话框，让用户选择一个样条曲线，然后在该样条曲线的每个定义点处创建一个点。
>
> 代码的主要流程如下：
>
> 1. 包含必要的头文件，定义宏用于打印错误信息和变量值。
> 2. 定义了一个错误报告函数report_error，用于在出错时打印错误信息。
> 3. 定义了一个函数mask_for_splines，用于设置选择掩码，使得只能选择样条曲线。
> 4. 定义了一个函数select_a_spline，用于弹出对话框让用户选择一个样条曲线，并返回该样条曲线的tag。
> 5. 定义了一个函数do_it，是主要功能的实现函数：循环调用select_a_spline函数，获取用户选择的样条曲线调用UF_CURVE_ask_spline_thru_pts函数获取样条曲线的定义点信息遍历所有定义点，调用UF_CURVE_create_point函数在每个定义点处创建一个点
> 6. 循环调用select_a_spline函数，获取用户选择的样条曲线
> 7. 调用UF_CURVE_ask_spline_thru_pts函数获取样条曲线的定义点信息
> 8. 遍历所有定义点，调用UF_CURVE_create_point函数在每个定义点处创建一个点
> 9. 定义了ufusr函数，是NX二次开发的入口函数：调用UF_initialize函数初始化NX环境调用do_it函数实现主要功能调用UF_terminate函数终止NX环境
> 10. 调用UF_initialize函数初始化NX环境
> 11. 调用do_it函数实现主要功能
> 12. 调用UF_terminate函数终止NX环境
> 13. 定义了ufusr_ask_unload函数，当NX卸载该二次开发代码时返回立即卸载。
>
> 综上，这段代码实现了在NX中选择一个样条曲线，并在其每个定义点处创建一个点的功能。
>
