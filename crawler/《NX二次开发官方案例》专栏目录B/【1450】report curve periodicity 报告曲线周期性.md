### 【1450】report curve periodicity 报告曲线周期性

#### 代码

```cpp
    /*HEAD REPORT_CURVE_PERIODICITY CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能。 */  
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
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves_and_edges, NULL,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t curve;  
        int status;  
        while ((curve=select_a_curve_or_edge("Select Edge or Curve")) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_curve_periodicity(curve, &status));  
            ECHO(status);  
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

> 这段代码是一个NX的二次开发示例，主要功能是检查曲线的周期性。具体来说：
>
> 1. 定义了一个错误报告函数report_error，用于输出错误信息。
> 2. 定义了一个用于选择曲线或边的函数select_a_curve_or_edge，通过用户对话框让用户选择一个曲线或边，并返回其tag。
> 3. 定义了一个函数do_it，用于不断选择曲线或边，并调用UF_MODL_ask_curve_periodicity函数检查其周期性，输出结果。
> 4. 定义了ufusr函数，作为主函数，初始化后调用do_it函数，然后终止。
> 5. 定义了ufusr_ask_unload函数，返回立即卸载标志。
> 6. 使用UF_print_syslog函数输出错误信息到系统日志，这是NX18版本新增的功能。
> 7. 使用UF_DISP_set_highlight函数高亮显示选中的对象。
> 8. 使用UF_UI_set_sel_mask函数设置选择掩码，只允许选择线、圆、圆锥曲线、样条线和实体边。
>
> 总体来说，这是一个简单的NX二次开发示例，实现了选择曲线或边并检查其周期性的功能，可以用于学习NX的二次开发。
>
