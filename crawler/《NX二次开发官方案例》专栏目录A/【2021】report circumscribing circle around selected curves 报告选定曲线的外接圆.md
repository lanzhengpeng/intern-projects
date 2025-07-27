### 【2021】report circumscribing circle around selected curves 报告选定曲线的外接圆

#### 代码

```cpp
    /*HEAD REPORT_CIRCUMSCRIBING_CIRCLE_AROUND_SELECTED_CURVES CCC UFUN */  
    /*  There is no API function to perform 2D analysis, so this example  
        demonstrates passing the selected curves to GRIP to use the  
        ANLSIS/TWOD command to obtain the circumscribing circle size and  
        center location.  
        Here is the code for the corresponding GRIP program "anlsis_twod.grs":  
        ENTITY/ curves(100), temp_b  
        NUMBER/ a_res(32), n_curves, cc_size, cc_xc, cc_yc  
        UFARGS/ curves, n_curves, cc_size, cc_xc, cc_yc  
        temp_b = BOUND/CLOSED,curves(1..n_curves)  
        ANLSIS/TWOD, temp_b, INCHES, a_res  
        DELETE/temp_b  
        cc_size = a_res(30)  
        cc_xc = a_res(31)  
        cc_yc = a_res(32)  
        HALT  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的提示，UF_print_syslog是V18版本中新增的功能。 */  
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
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, UF_all_subtype, 0 },  
                        { UF_spline_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_curves(char *prompt, tag_t **curves)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select curves", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_curves, NULL, &resp, &cnt, curves));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*curves)[ii], FALSE));  
        return cnt;  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_curves;  
        tag_t  
            *curves,  
            g_curves[100];  
        double  
            g_n_curves,  
            diameter,  
            center_xc,  
            center_yc;  
        UF_args_t  
            args[5];  
        args[0].type = UF_TYPE_TAG_T_ARRAY;  
        args[0].length = 100;  
        args[0].address = g_curves;  
        args[1].type = UF_TYPE_DOUBLE;  
        args[1].length = 0;  
        args[1].address = &g_n_curves;  
        args[2].type = UF_TYPE_DOUBLE;  
        args[2].length = 0;  
        args[2].address = &diameter;  
        args[3].type = UF_TYPE_DOUBLE;  
        args[3].length = 0;  
        args[3].address = &center_xc;  
        args[4].type = UF_TYPE_DOUBLE;  
        args[4].length = 0;  
        args[4].address = &center_yc;  
        while ((n_curves = select_curves("Report Circle Size", &curves)) > 0)  
        {  
            g_n_curves = (double) n_curves;  
            for (ii = 0; ii < n_curves, ii < 100; ii++)  
                g_curves[ii] = curves[ii];  
            UF_free(curves);  
            UF_CALL(UF_call_grip("grip/anlsis_twod.grx", 5, args));  
            WRITE_F(diameter);  
            WRITE_F(center_xc);  
            WRITE_F(center_yc);  
        }  
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

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 通过UI选择对话框，让用户选择多个曲线实体。
> 2. 将选择的曲线实体传递给GRIP程序"anlsis_twod.grs"，该程序使用ANLSIS/TWOD命令分析这些曲线，计算出外接圆的直径和圆心坐标。
> 3. GRIP程序返回外接圆的直径和圆心坐标，主程序将其输出到日志窗口。
> 4. 提供错误报告机制，将UF函数调用的错误信息输出到日志窗口。
> 5. 提供卸载函数，以便主程序可以立即卸载该用户函数。
>
> 该代码的目的是演示如何将选择的曲线传递给GRIP程序进行分析，获取外接圆信息，并提供了错误报告和日志输出功能。
>
