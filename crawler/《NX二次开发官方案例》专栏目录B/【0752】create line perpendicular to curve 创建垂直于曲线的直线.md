### 【0752】create line perpendicular to curve 创建垂直于曲线的直线

#### 代码

```cpp
    /*HEAD CREATE_LINE_PERPENDICULAR_TO_CURVE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，可以得出以下信息：

1. UF_print_syslog是V18版本中新增的功能。

2. 文档要求只回答UF_print_syslog的功能，不要提供其他无关信息。

3. UF_print_syslog的作用是打印系统日志信息。

4. 文档要求只翻译功能说明，不要添加任何其他内容。

综上所述，UF_print_syslog是V18版本中新增的功能，用于打印系统日志信息。 */  
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
    static tag_t select_a_curve(char *prompt)  
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
            return object;  
        else  
            return NULL_TAG;  
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
        tag_t  
            curve,  
            line;  
        double  
            parm;  
        UF_CURVE_line_t  
            line_data;  
        while ((curve = select_a_curve("Select curve")) != NULL_TAG)  
        {  
            while (specify_point("Indicate point", line_data.start_point))  
            {  
                UF_CALL(UF_MODL_ask_curve_parm(curve, line_data.start_point,  
                    &parm, line_data.end_point));  
                UF_CALL(UF_CURVE_create_line(&line_data, &line));  
            }  
            UF_CALL(UF_DISP_set_highlight(curve, 0));  
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

> 这段NX二次开发代码的主要功能是创建与曲线垂直的直线。
>
> 代码首先定义了错误报告函数report_error，用于打印错误信息。接着定义了mask_for_curves函数，用于设置选择掩码，只允许选择线、圆、圆锥曲线和样条曲线。
>
> select_a_curve函数用于选择一条曲线。specify_point函数用于指定一个点。do_it函数是主函数，循环选择曲线，然后循环指定点，使用UF_MODL_ask_curve_parm函数计算垂直参数，使用UF_CURVE_create_line函数创建垂直于曲线的直线。
>
> 最后，ufusr函数是NX的入口函数，调用do_it函数执行创建操作。ufusr_ask_unload函数用于卸载NX的二次开发代码。
>
> 综上所述，这段代码实现了创建与曲线垂直的直线功能，通过选择曲线和指定点，循环创建垂直直线，并提供了错误报告和卸载功能。
>
